//
// Created by EgrZver on 04.10.2023.
//
#include <iostream>
#include "Board.h"

namespace CLCEngine {

    Board::Board(CheckerColor playerSide) {
        this->currentMover = CheckerColor::White;
        this->playerSide = playerSide;
        this->availableSequences = {};
        this->nearestAvailableMoves = {};
        this->checkersCount[0] = 12;
        this->checkersCount[1] = 12; //??

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                this->gameBoard[i][j].assignChecker(nullptr);
            }
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 8; j += 2) {
                Checker
                *white = new Checker(
                        {
                            {j + (i % 2), i},
                            CheckerColor::White,
                            CheckerType::Common,
                            false
                        }
                    ),
                *black = new Checker(
                        {
                            {j + 1 - (i % 2), 7 - i},
                            CheckerColor::Black,
                            CheckerType::Common,
                            false
                        }
                    );
                this->gameBoard[i][j + (i % 2)].assignChecker(white);
                this->gameBoard[7 - i][j + 1 - (i % 2)].assignChecker(black);
            }
        }
    }

    Board::Board(CheckerColor playerSide, CheckerColor currentMover, int* checkersCountArray) {
        this->currentMover = currentMover;
        this->playerSide = playerSide;
        this->availableSequences = {};
        this->nearestAvailableMoves = {};
        this->checkersCount[0] = checkersCountArray[0];
        this->checkersCount[1] = checkersCountArray[1];

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                this->gameBoard[i][j].assignChecker(nullptr);
            }
        }
    }

    Board::~Board() {
        this->availableSequences.clear();
        this->nearestAvailableMoves.clear();
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                this->gameBoard[i][j].destroyVillager(true);
            }
        }
    }

    void Board::assignCheckerExternal(CheckerMetaInfo metaInfo) {
        Coordinates place = metaInfo.coordinates;
        Checker* assignated = new Checker(metaInfo);
        this->gameBoard[place.y][place.x].assignChecker(assignated);
    }

    MoveSequencesList Board::getAvailableSequences() {
        return this->availableSequences;
    }

    MoveList Board::getNearestAvailableMoves() {
        return this->nearestAvailableMoves;
    }

    void Board::makeAMove(Move move) {
        Coordinates src = move.getSource(), dst = move.getDestination();
        Checker* moving = this->gameBoard[src.y][src.x].extractChecker();
        moving->applyMove(move);
        //this->checkersCount[(int) moving->getMetaInfo().color]--;
        this->gameBoard[dst.y][dst.x].assignChecker(moving);
        this->gameBoard[src.y][src.x].destroyVillager(false);
        CheckerMetaInfo victim = move.getVictimMetaInfo();
        if (!(victim == NO_VICTIM)) {
            this->gameBoard[victim.coordinates.y][victim.coordinates.x].destroyVillager(true);
            this->checkersCount[(int) victim.color]--;
        }
    }

    void Board::unmakeAMove(Move move) {
        Coordinates src = move.getSource(), dst = move.getDestination();
        Checker* moving = this->gameBoard[dst.y][dst.x].extractChecker();
        moving->unapplyMove(move);
        //this->checkersCount[(int) moving->getMetaInfo().color]++;
        this->gameBoard[src.y][src.x].assignChecker(moving);
        this->gameBoard[dst.y][dst.x].destroyVillager(false);
        CheckerMetaInfo victim = move.getVictimMetaInfo();
        if (!(victim == NO_VICTIM)) {
            Checker* regenerated = new Checker(victim);
            this->gameBoard[victim.coordinates.y][victim.coordinates.x].assignChecker(regenerated);
            this->checkersCount[(int) victim.color]++;
        }
    }

    void Board::makeASequence(MoveList& sequence) {
        for (int i = 0; i < sequence.getLength(); i++) {
            this->makeAMove(sequence.getElement(i));
        }
    }

    void Board::unmakeASequence(MoveList& sequence) {
        for (int i = sequence.getLength() - 1; i > -1; i--) {
            this->unmakeAMove(sequence.getElement(i));
        }
    }

    void Board::flushLists() {
        this->availableSequences.clear();
        this->nearestAvailableMoves.clear();
    }

    void Board::findAvailableMoves() {
        MetaArray metaArray = this->makeMetaArray();
        bool capturingOnly = false, firstWasCommon = false;
        MoveSequencesList buffer;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                //std::cout << this->gameBoard[i][j].extractChecker() << std::endl;
                if (!this->gameBoard[i][j].empty()) {
                    if (this->gameBoard[i][j].getInternalCheckerColor() == this->currentMover) {
                        MoveSequencesList ofOne = this->gameBoard[i][j].getSequencesOfVillager(metaArray);
                        for (int k = 0; k < ofOne.getLength(); k++) {
                            buffer.append(ofOne.getElement(k));
                            //this->nearestAvailableMoves.append(ofOne.getElement(k).getElement(0));
                        }
                    }

                }
            }
        }

        for (int i = 0; i < buffer.getLength(); i++) {
            if (!(buffer.getElement(i).getElement(0).getVictimMetaInfo() == NO_VICTIM)) {
                capturingOnly = true;
                break;
            }
        }

        for (int i = 0; i < buffer.getLength(); i++) {
            if (capturingOnly) {
                if (!(buffer.getElement(i).getElement(0).getVictimMetaInfo() == NO_VICTIM)) {
                    this->availableSequences.append(buffer.getElement(i));
                    this->nearestAvailableMoves.append(buffer.getElement(i).getElement(0));
                }
            } else {
                this->availableSequences.append(buffer.getElement(i));
                this->nearestAvailableMoves.append(buffer.getElement(i).getElement(0));
            }

        }

        for (int i = 0; i < 8; i++) {
            delete [] metaArray[i];
        }
        delete [] metaArray;
    }

    bool Board::noMovesNow() {
        return this->availableSequences.getLength() + this->nearestAvailableMoves.getLength() == 0;

    }
    void Board::passTurn() {
        if (this->currentMover == CheckerColor::Black) {
            this->currentMover = CheckerColor::White;
        } else {
            this->currentMover = CheckerColor::Black;
        }
    }

    CheckerColor Board::whoMoves() {
        return this->currentMover;
    }

    CheckerColor Board::whoIsPlayer() {
        return this->playerSide;
    }

    MetaArray Board::makeMetaArray() {
        MetaArray meta = new CheckerMetaInfo* [8];

        for (int i = 0; i < 8; i++) {
            meta[i] = new CheckerMetaInfo[8];
            for (int j = 0; j < 8; j++) {

                if (this->gameBoard[i][j].empty()) {
                    meta[i][j] = {{j, i}, CheckerColor::NoColor, CheckerType::NoType, false};
                } else {
                    CheckerMetaInfo extracted = this->gameBoard[i][j].extractChecker()->getMetaInfo();
                    meta[i][j] = {{j, i}, extracted.color, extracted.type, extracted.markedForDeath};
                }
            }
        }

        return meta;

    }

    CheckerMetaInfo Board::getMetaInfo(Coordinates onBoardPlace) {
        if (this->gameBoard[onBoardPlace.y][onBoardPlace.x].empty()) {
            return {onBoardPlace, CheckerColor::NoColor, CheckerType::NoType, false};
        } else {
            return this->gameBoard[onBoardPlace.y][onBoardPlace.x].extractChecker()->getMetaInfo();
        }
    }

    void Board::makeDeepCopy(const Board &source) {
        this->currentMover = source.currentMover;
        this->playerSide = source.playerSide;
        this->availableSequences = source.availableSequences;
        this->nearestAvailableMoves = source.nearestAvailableMoves;
        this->checkersCount[0] = source.checkersCount[0];
        this->checkersCount[1] = source.checkersCount[1];

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                this->gameBoard[i][j].destroyVillager(true);
                if (!source.gameBoard[i][j].empty()) {
                    CheckerMetaInfo other = source.gameBoard[i][j].extractChecker()->getMetaInfo();
                    Checker* copy = new Checker(other);
                    this->gameBoard[i][j].assignChecker(copy);
                }

            }
        }
    }

    Board::Board(const Board &source) {
        this->makeDeepCopy(source);
    }

    Board &Board::operator=(const Board &source) {
        if (this != &source) {
            this->makeDeepCopy(source);
        }
        return *this;
    };

    Board::Board(Board&& moved) {
        this->currentMover = moved.currentMover;
        this->playerSide = moved.playerSide;
        this->availableSequences = moved.availableSequences;
        this->nearestAvailableMoves = moved.nearestAvailableMoves;
        this->checkersCount[0] = moved.checkersCount[0];
        this->checkersCount[1] = moved.checkersCount[1];

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                this->gameBoard[i][j].assignChecker(moved.gameBoard[i][j].extractChecker());
                moved.gameBoard[i][j].destroyVillager(false);
            }
        }
    }

    Coordinates Board::getCheckersCount() {
        return {this->checkersCount[0], this->checkersCount[1]};
    }


} // CLCEngine
