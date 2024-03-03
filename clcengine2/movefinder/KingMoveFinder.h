//
// Created by EgrZver on 04.10.2023.
//
#include "AbstractMoveFinder.h"

#ifndef CLCHECKERS2_KINGMOVEFINDER_H
#define CLCHECKERS2_KINGMOVEFINDER_H

namespace CLCEngine {

    class KingMoveFinder: public AbstractMoveFinder {
        protected:
            int findDistanceToVictim(
                MetaArray metaArray,
                Coordinates source,
                int dirH,
                int dirV);
            bool longMoveLookingFlag(
                Coordinates place,
                int dirH,
                int dirV
            );
        public:
            MoveList getNearestCommonMoves(MetaArray metaArray,
                                           Coordinates forWhat) override;
            MoveList getNearestCapturingMoves(MetaArray metaArray,
                                              Coordinates forWhat) override;
    };

} // CLCEngine

#endif //CLCHECKERS2_KINGMOVEFINDER_H
