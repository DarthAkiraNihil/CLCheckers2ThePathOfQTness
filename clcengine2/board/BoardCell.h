//
// Created by EgrZver on 04.10.2023.
//
#include <clcengine2/checker/Checker.h>

#ifndef CLCHECKERS2_BOARDCELL_H
#define CLCHECKERS2_BOARDCELL_H

namespace CLCEngine {

    class BoardCell {
        protected:
            Checker* checker;
        public:
            BoardCell();
            bool empty() const;
            MoveSequencesList getSequencesOfVillager(MetaArray metaArray);
            Checker* extractChecker() const;
            void assignChecker(Checker* what);
            void destroyVillager(bool forced);
            CheckerColor getInternalCheckerColor() const;
    };

} // CLCEngine

#endif //CLCHECKERS2_BOARDCELL_H
