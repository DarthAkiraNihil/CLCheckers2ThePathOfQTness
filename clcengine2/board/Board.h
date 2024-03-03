//
// Created by EgrZver on 04.10.2023.
//
#include "BoardCell.h"

#ifndef CLCHECKERS2_BOARD_H
#define CLCHECKERS2_BOARD_H

namespace CLCEngine {

    class Board {
        protected:
            BoardCell gameBoard[8][8];
            MoveSequencesList availableSequences;
            MoveList nearestAvailableMoves;
            int checkersCount[2];
            CheckerColor currentMover, playerSide;


            void makeDeepCopy(const Board &source);

        public:
            explicit Board(CheckerColor playerSide);
            Board(CheckerColor playerSide, CheckerColor curruentMover, int* checkersCountArray);

            Board(const Board &source);
            Board(Board&& moved);

            ~Board();
            MetaArray makeMetaArray();

            MoveSequencesList getAvailableSequences();
            MoveList getNearestAvailableMoves();

            void makeAMove(Move move);
            void unmakeAMove(Move move);

            void makeASequence(MoveList& sequence);
            void unmakeASequence(MoveList& sequence);

            void flushLists();

            void findAvailableMoves();

            bool noMovesNow();
            void passTurn();

            CheckerColor whoMoves();
            CheckerColor whoIsPlayer();

            Coordinates getCheckersCount();
            void assignCheckerExternal(CheckerMetaInfo metaInfo);

            CheckerMetaInfo getMetaInfo(Coordinates onBoardPlace);

            //Coordinates getSquareNotationCoordinates(Coordinates onBoardPlace);

            Board& operator=(const Board& source);
    };

} // CLCEngine

#endif //CLCHECKERS2_BOARD_H
