//
// Created by EgrZver on 05.10.2023.
//
#include <random>
#include <iostream>
#include "EvalTables.h"
#include "ComputerRivalMinimax.h"

namespace CLCEngine {

    std::random_device analyzerRD;
    std::mt19937 agen(analyzerRD());
    std::uniform_int_distribution<> analyzerDistribution(1, 1000);

    ComputerRivalMinimax::ComputerRivalMinimax(CLCEngine::Difficulty difficulty, CLCEngine::CheckerColor side) {
        this->maxSearchDepth = difficulty;
        this->side = side;
        this->commonCost = 100;
        this->kingCost = 250;
        for (int i = 0; i < 4;i++) {
            for (int j = 0; j < 8;j++) {
                if (difficulty == Difficulty::Extreme) {
                    this->evalTable[i][j] = EvalTables::eggman[i][j];
                } else {
                    this->evalTable[i][j] = EvalTables::std[i][j];
                }

            }
        }
        //this->evalTable = EvalTables::std;
    }

    double ComputerRivalMinimax::eval(CLCEngine::MetaArray metaArray) {

        double eval = 0.0;

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (metaArray[i][j].type == CheckerType::King) {
                    if (metaArray[i][j].color == CheckerColor::White) {
                        eval -= (double) this->kingCost;
                    } else {
                        eval += (double) this->kingCost;
                    }
                } else {
                    if (metaArray[i][j].color == CheckerColor::White) {
                        eval -= this->commonCost * this->evalTable[7 - i][(7 - j) / 2];
                    } else {
                        eval += this->commonCost * this->evalTable[i][j / 2];
                    }
                }
            }
        }
        //std::cout << "EVAL: " << eval << std::endl;
        return eval;
    }

    AnalyzerContainer ComputerRivalMinimax::analyze(CLCEngine::Board *board, MetaArray boardMetaArray, int currentDepth) {
        AnalyzerContainer toReturn; toReturn.sequence = {}; toReturn.eval = 0.0;
        double topBorder = (board->whoMoves() == CheckerColor::White) ? 100000.0 : -100000.0;
        //toReturn.eval = topBorder;
        board->findAvailableMoves();

        MoveSequencesList nearest = board->getAvailableSequences();
        if (board->noMovesNow()) {
            toReturn.eval = topBorder;
            return toReturn;
        }
        if (currentDepth == (int) this->maxSearchDepth - 1) {

            MetaArray temp = board->makeMetaArray();

            toReturn.eval = this->eval(temp);

            for (int i = 0; i < 8; i++) {
                delete [] temp[i];
            }
            delete [] temp;
            return toReturn;
        }
        for (int i = 0; i < nearest.getLength(); i++) {
            MoveList current = nearest.getElement(i);
            board->makeASequence(current);
            board->flushLists();
            board->passTurn();
            AnalyzerContainer deepDarkFantasy = this->analyze(board, boardMetaArray, currentDepth + 1);
            board->unmakeASequence(current);
            board->passTurn();
            if (
                (board->whoMoves() == CheckerColor::White) ?
                (deepDarkFantasy.eval < topBorder) :
                (deepDarkFantasy.eval > topBorder)
            ) {
                topBorder = deepDarkFantasy.eval;
                toReturn.eval = topBorder;
                toReturn.sequence = nearest.getElement(i);
            } else if (deepDarkFantasy.eval == topBorder) {
                if (analyzerDistribution(agen) < 500) {
                    toReturn.sequence = nearest.getElement(i);
                }
            }
        }
        return toReturn;
    }

} // CLCEngine
