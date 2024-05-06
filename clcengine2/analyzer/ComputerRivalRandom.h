//
// Created by EgrZver on 07.10.2023.
//
#include "AbstractComputerRival.h"

#ifndef CLCHECKERS2_COMPUTERRIVALRANDOM_H
#define CLCHECKERS2_COMPUTERRIVALRANDOM_H

namespace CLCEngine {

    class ComputerRivalRandom: public AbstractComputerRival {
        protected:
            AnalyzerContainer analyze(CLCEngine::Board *board, MetaArray boardMetaArray, int currentDepth) override;
            double eval(CLCEngine::MetaArray metaArray) override;
        public:
            ComputerRivalRandom(CheckerColor side);
    };

} // CLCEngine

#endif //CLCHECKERS2_COMPUTERRIVALRANDOM_H
