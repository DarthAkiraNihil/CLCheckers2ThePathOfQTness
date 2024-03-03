//
// Created by EgrZver on 04.10.2023.
//
#include "AbstractMoveFinder.h"

#ifndef CLCHECKERS2_COMMONMOVEFINDER_H
#define CLCHECKERS2_COMMONMOVEFINDER_H

namespace CLCEngine {

    class CommonMoveFinder: public AbstractMoveFinder {
        public:
            MoveList getNearestCapturingMoves(MetaArray metaArray,
                                              Coordinates forWhat) override;
            MoveList getNearestCommonMoves(MetaArray metaArray,
                                           Coordinates forWhat) override;
    };

} // CLCEngine

#endif //CLCHECKERS2_COMMONMOVEFINDER_H
