//
// Created by EgrZver on 04.10.2023.
//

#include "BoardCell.h"

namespace CLCEngine {

    BoardCell::BoardCell() {
        this->checker = nullptr;
    }

    bool BoardCell::empty() const {
        return this->checker == nullptr;
    }

    MoveSequencesList BoardCell::getSequencesOfVillager(MetaArray metaArray) {
        if (!this->empty()) {
            return this->checker->getAllSequences(metaArray);
        }
        return {};
    }
    Checker* BoardCell::extractChecker() const {
        return this->checker;
    }

    void BoardCell::assignChecker(Checker* what) {
        this->checker = what;
    }

    void BoardCell::destroyVillager(bool forced) {
        if (forced && this->checker) {
            delete this->checker;
        }
        this->checker = nullptr;
    }

    CheckerColor BoardCell::getInternalCheckerColor() const {
        return this->checker->getMetaInfo().color;
    }

} // CLCEngine