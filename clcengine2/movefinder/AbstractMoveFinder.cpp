//
// Created by EgrZver on 04.10.2023.
//

#include "AbstractMoveFinder.h"

namespace CLCEngine {
    AbstractMoveFinder::~AbstractMoveFinder() {

    }

    bool AbstractMoveFinder::isAVictim(CLCEngine::MetaArray metaArray,
                                       Coordinates forWhat, Coordinates where) {
        if (metaArray[forWhat.y][forWhat.x].color == CheckerColor::White) {
            return metaArray[where.y][where.x].color == CheckerColor::Black;
        } else if (metaArray[forWhat.y][forWhat.x].color == CheckerColor::Black) {
            return metaArray[where.y][where.x].color == CheckerColor::White;
        } else {
            return false;
        }
    }

    bool AbstractMoveFinder::isAFriend(MetaArray metaArray,  Coordinates forWhat, Coordinates where) {
        return metaArray[where.y][where.x].color == metaArray[forWhat.y][forWhat.x].color;
    }
    bool AbstractMoveFinder::isNothingHere(MetaArray metaArray, Coordinates where) {
        return
            metaArray[where.y][where.x].color == CheckerColor::NoColor &&
            metaArray[where.y][where.x].type == CheckerType::NoType;
    }

    bool AbstractMoveFinder::isMarkedForDeathMeta(MetaArray metaArray, Coordinates where) {
        if (this->isNothingHere(metaArray, where)) return false;
        return metaArray[where.y][where.x].markedForDeath;
    }

} // CLCEngine