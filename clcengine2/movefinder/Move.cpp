//
// Created by EgrZver on 03.10.2023.
//

#include "Move.h"

namespace CLCEngine {
    Move::Move() {
        this->source = {-1, -1};
        this->destination = {-1, -1};
        this->isMoveSpecial = false;
        this->victimMetaInfo = NO_VICTIM;
    }

    Move::Move(CLCEngine::Coordinates source, CLCEngine::Coordinates destination, bool isMoveSpecial) {
        this->source = source;
        this->destination = destination;
        this->isMoveSpecial = isMoveSpecial;
        this->victimMetaInfo = NO_VICTIM;
    }

    void Move::setVictim(CheckerMetaInfo victimMeta) {
        this->victimMetaInfo = victimMeta;
    }

    Coordinates Move::getSource() {
        return this->source;
    }
    Coordinates Move::getDestination() {
        return this->destination;
    }
    CheckerMetaInfo Move::getVictimMetaInfo() {
        return this->victimMetaInfo;
    }

    bool Move::isSpecial() {
        return this->isMoveSpecial;
    }
} // CLCEngine