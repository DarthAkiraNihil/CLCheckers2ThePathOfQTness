//
// Created by EgrZver on 04.10.2023.
//
#include <clcengine2/auxiliary/DynamicSequence.h>
#include <clcengine2/checker/CheckerMetaInfo.h>

#ifndef CLCHECKERS2_ABSTRACTMOVEFINDER_H
#define CLCHECKERS2_ABSTRACTMOVEFINDER_H

namespace CLCEngine {

    class AbstractMoveFinder {
        public:
            virtual ~AbstractMoveFinder() = 0;

            virtual MoveList getNearestCommonMoves(
                MetaArray metaArray,
                Coordinates forWhat
            ) = 0;
            virtual MoveList getNearestCapturingMoves(
                MetaArray metaArray,
                Coordinates forWhat
            ) = 0;

            bool isAVictim(MetaArray metaArray, Coordinates forWhat, Coordinates where);
            bool isAFriend(MetaArray metaArray, Coordinates forWhat, Coordinates where);
            bool isNothingHere(MetaArray metaArray, Coordinates where);

            bool isMarkedForDeathMeta(MetaArray metaArray, Coordinates where);
    };

} // CLCEngine

#endif //CLCHECKERS2_ABSTRACTMOVEFINDER_H
