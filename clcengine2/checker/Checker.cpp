//
// Created by EgrZver on 04.10.2023.
//



#include "Checker.h"

#include <clcengine2/movefinder/CommonMoveFinder.h>
#include <clcengine2/movefinder/KingMoveFinder.h>

namespace CLCEngine {

    Checker::Checker(CLCEngine::CheckerMetaInfo checkerMetaInfo) {
        this->metaInfo = checkerMetaInfo;
        if (checkerMetaInfo.type == CheckerType::Common) {
            this->moveFinder = new CommonMoveFinder;
        } else {
            this->moveFinder = new KingMoveFinder;
        }
    }

    void Checker::applyMoveMeta(MetaArray metaArray, Move move) {
        this->applyMove(move);

        Coordinates src = move.getSource(), dst = move.getDestination();
        metaArray[dst.y][dst.x] = metaArray[src.y][src.x];

        metaArray[src.y][src.x].color = CheckerColor::NoColor;
        metaArray[src.y][src.x].type = CheckerType::NoType;
        metaArray[src.y][src.x].markedForDeath = false;

        metaArray[dst.y][dst.x].coordinates = dst;
        CheckerMetaInfo victim = move.getVictimMetaInfo();
        if (!(victim == NO_VICTIM)) {
            metaArray[victim.coordinates.y][victim.coordinates.x].markedForDeath = true;
        }
    }

    void Checker::unapplyMoveMeta(MetaArray metaArray, Move move) {
        this->unapplyMove(move);

        Coordinates src = move.getSource(), dst = move.getDestination();
        metaArray[src.y][src.x] = metaArray[dst.y][dst.x];

        metaArray[dst.y][dst.x].color = CheckerColor::NoColor;
        metaArray[dst.y][dst.x].type = CheckerType::NoType;
        metaArray[dst.y][dst.x].markedForDeath = false;

        metaArray[src.y][src.x].coordinates = src;
        if (move.isSpecial()) {
            this->descend();
        }
        CheckerMetaInfo victim = move.getVictimMetaInfo();
        if (!(victim == NO_VICTIM)) {
            metaArray[victim.coordinates.y][victim.coordinates.x].markedForDeath = false;
        }
    }

    CheckerMetaInfo Checker::getMetaInfo() {
        return this->metaInfo;
    }

    void Checker::findAllCommonSequences(
        MoveSequencesList* destination,
        MetaArray metaArray
    ) {

        MoveList nearest = this->moveFinder->getNearestCommonMoves(metaArray, this->metaInfo.coordinates), current;
        for (int i = 0; i < nearest.getLength(); i++) {
            Move currentMove = nearest.getElement(i);
            current.append(currentMove);
            if (currentMove.isSpecial()) {
                this->applyMoveMeta(metaArray, currentMove);
                MoveList capturing = this->moveFinder->getNearestCapturingMoves(metaArray, this->metaInfo.coordinates);
                if (capturing.getLength() == 0) {
                    MoveList stepTwo = this->moveFinder->getNearestCommonMoves(metaArray, this->metaInfo.coordinates);
                    for (int j = 0; j < stepTwo.getLength(); j++) {
                        current.append(stepTwo.getElement(j));
                        destination->append(current);
                        current.pop();
                    }
                } else {
                    destination->append(current);
                }
                this->unapplyMoveMeta(metaArray, currentMove);
            } else {
                destination->append(current);
            }
            current.pop();
        }

    }

    void Checker::findAllCapturingSequences(
        MoveSequencesList* destination,
        MetaArray metaArray,
        MoveList* currentPath,
        bool afterMove
    ) {

        MoveList nearest = this->moveFinder->getNearestCapturingMoves(metaArray, this->metaInfo.coordinates);

        if (afterMove && nearest.getLength() == 0) {
            destination->append(*currentPath);
            return;
        }

        for (int i = 0; i < nearest.getLength(); i++) {
            Move current = nearest.getElement(i);
            currentPath->append(current);

            this->applyMoveMeta(metaArray, current);
            this->findAllCapturingSequences(destination, metaArray, currentPath, ENABLED_EMULATION);
            this->unapplyMoveMeta(metaArray, current);

            currentPath->pop();

        }
    }

    MoveSequencesList Checker::getAllSequences(MetaArray metaArray) {
        MoveSequencesList capturing; MoveList path;
        this->findAllCapturingSequences(&capturing, metaArray, &path);
        if (capturing.getLength() != 0) {
            return capturing;
        }
        MoveSequencesList common;
        this->findAllCommonSequences(&common, metaArray);
        if (common.getLength() != 0) return common;
        else return {};
    }

    MoveList Checker::getNearestMoves(MetaArray metaArray) {
        MoveList nearestCapt = this->moveFinder->getNearestCapturingMoves(metaArray, this->metaInfo.coordinates);
        if (nearestCapt.getLength() != 0) {
            return nearestCapt;
        }
        return this->moveFinder->getNearestCommonMoves(metaArray, this->metaInfo.coordinates);
    }

    void Checker::applyMove(Move move) {
        this->metaInfo.coordinates = move.getDestination();
        if (move.isSpecial()) {
            this->ascend();
        }
    }

    void Checker::unapplyMove(Move move) {
        this->metaInfo.coordinates = move.getSource();
        if (move.isSpecial()) {
            this->descend();
        }
    }

    void Checker::markForDeath() {
        this->metaInfo.markedForDeath = true;
    }

    void Checker::unmarkForDeath() {
        this->metaInfo.markedForDeath = false;
    }

    void Checker::ascend() {
        this->metaInfo.type = CheckerType::King;
        delete this->moveFinder;
        this->moveFinder = new KingMoveFinder;
    }

    void Checker::descend() {
        this->metaInfo.type = CheckerType::Common;
        delete this->moveFinder;
        this->moveFinder = new CommonMoveFinder;
    }

} // CLCEngine