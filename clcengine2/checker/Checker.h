//
// Created by EgrZver on 04.10.2023.
//
#include <clcengine2/auxiliary/DynamicSequence.h>
#include <clcengine2/movefinder/AbstractMoveFinder.h>

#ifndef CLCHECKERS2_CHECKER_H
#define CLCHECKERS2_CHECKER_H

namespace CLCEngine {
    const bool ENABLED_EMULATION = true;

    class Checker {
        protected:
            CheckerMetaInfo metaInfo;
            AbstractMoveFinder* moveFinder;
            //bool markedForDeath;

            void findAllCommonSequences(
                MoveSequencesList* destination,
                MetaArray metaArray
            );
            void findAllCapturingSequences(
                MoveSequencesList* destination,
                MetaArray metaArray,
                MoveList* currentPath,
                bool afterMove = false
            );

            void applyMoveMeta(MetaArray metaArray, Move move);
            void unapplyMoveMeta(MetaArray metaArray, Move move);

        public:
            Checker(CheckerMetaInfo checkerMetaInfo);
            //AbstractChecker(CheckerMetaInfo meta);

            CheckerMetaInfo getMetaInfo();
            //bool isMarkedForDeath();

            MoveSequencesList getAllSequences(MetaArray metaArray);
            MoveList getNearestMoves(MetaArray metaArray);

            void applyMove(Move move);
            void unapplyMove(Move move);

            void markForDeath();
            void unmarkForDeath();

            void ascend();
            void descend();
    };

} // CLCEngine

#endif //CLCHECKERS2_CHECKER_H
