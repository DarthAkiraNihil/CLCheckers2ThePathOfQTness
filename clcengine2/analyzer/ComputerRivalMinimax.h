//
// Created by EgrZver on 05.10.2023.
//
#include "AbstractComputerRival.h"

#ifndef CLCHECKERS2_COMPUTERRIVALMINIMAX_H
#define CLCHECKERS2_COMPUTERRIVALMINIMAX_H



namespace CLCEngine {



    class ComputerRivalMinimax: public AbstractComputerRival {
        protected:
            double evalTable[8][4];/* = {
                {1.2, 1.2, 1.2, 1.2},
                {1.15, 1.2, 1.2, 1.15},
                {1.15, 1.2, 1.2, 1.13},
                {1.0, 1.2, 1.15, 1.0},
                {1.0, 1.2, 1.2, 1.0},
                {1.0, 1.0, 1.0, 1.0},
                {1.0, 1.0, 1.0, 1.0},
                {1.0, 1.0, 1.0, 1.0},
            };*/
            AnalyzerContainer analyze(CLCEngine::Board *board, MetaArray boardMetaArray, int currentDepth) override;
            double eval(CLCEngine::MetaArray metaArray) override;
            int commonCost, kingCost;
        public:
            ComputerRivalMinimax(Difficulty difficulty, CheckerColor side);
    };

} // CLCEngine

#endif //CLCHECKERS2_COMPUTERRIVALMINIMAX_H
