//
// Created by EgrZver on 05.10.2023.
//

#include "AbstractComputerRival.h"

namespace CLCEngine {

    MoveList AbstractComputerRival::getNextSequence(CLCEngine::Board board) {
        MetaArray metaArray = board.makeMetaArray();
        AnalyzerContainer container = this->analyze(&board, metaArray, 0);
        for (int i = 0; i < 8; i++) {
            delete [] metaArray[i];
        }
        delete [] metaArray;
        board.flushLists();
        return container.sequence;
    }

    AbstractComputerRival::~AbstractComputerRival() {

    }

    CheckerColor AbstractComputerRival::getSide() {
        return this->side;
    }

    Difficulty AbstractComputerRival::getMaxDepth() {
        return this->maxSearchDepth;
    }

} // CLCEngine
