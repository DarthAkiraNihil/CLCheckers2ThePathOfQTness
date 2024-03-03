//
// Created by EgrZver on 05.10.2023.
//
#include "Difficulty.h"
#include "AnalyzerContainer.h"
#include <clcengine2/board/Board.h>

#ifndef CLCHECKERS2_ABSTRACTCOMPUTERRIVAL_H
#define CLCHECKERS2_ABSTRACTCOMPUTERRIVAL_H

namespace CLCEngine {

    class AbstractComputerRival {
        protected:
            Difficulty maxSearchDepth;
            CheckerColor side;

            virtual AnalyzerContainer analyze(
                Board* board,
                MetaArray boardMetaArray,
                int currentDepth
                ) = 0;

            virtual double eval(MetaArray metaArray) = 0;

        public:
            MoveList getNextSequence(Board board);
            virtual ~AbstractComputerRival();

            CheckerColor getSide();
            Difficulty getMaxDepth();
    };

} // CLCEngine

#endif //CLCHECKERS2_ABSTRACTCOMPUTERRIVAL_H
