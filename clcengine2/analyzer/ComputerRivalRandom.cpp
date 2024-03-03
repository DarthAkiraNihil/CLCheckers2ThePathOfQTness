//
// Created by EgrZver on 07.10.2023.
//
#include <random>
#include "ComputerRivalRandom.h"

namespace CLCEngine {
    ComputerRivalRandom::ComputerRivalRandom(CLCEngine::CheckerColor side) {
        this->side = side;
        this->maxSearchDepth = Difficulty::Dumbass;
    }

    double ComputerRivalRandom::eval(CLCEngine::MetaArray metaArray) {
        return 0.0;
    }

    AnalyzerContainer ComputerRivalRandom::analyze(CLCEngine::Board *board, CLCEngine::MetaArray boardMetaArray,
                                                   int currentDepth) {
        board->findAvailableMoves();
        MoveSequencesList nearest = board->getAvailableSequences();
        std::random_device decider;
        std::mt19937 gen(decider());
        std::uniform_int_distribution<> dist(0, nearest.getLength() - 1);
        return {nearest.getElement(dist(gen)), 0.0};
    }

} // CLCEngine