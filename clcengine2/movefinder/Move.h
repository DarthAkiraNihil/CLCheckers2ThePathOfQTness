//
// Created by EgrZver on 03.10.2023.
//
#include <clcengine2/checker/CheckerMetaInfo.h>

#ifndef CLCENGINE2_MOVE_H
#define CLCENGINE2_MOVE_H

namespace CLCEngine {

    class Move {
        private:
            Coordinates source{}, destination{};
            CheckerMetaInfo victimMetaInfo{};
            bool isMoveSpecial;
        public:
            Move();
            Move(Coordinates source,
                 Coordinates destination,
                 bool isMoveSpecial);

            void setVictim(CheckerMetaInfo victimMeta);

            Coordinates getSource();
            Coordinates getDestination();
            CheckerMetaInfo getVictimMetaInfo();

            bool isSpecial();
    };

} // CLCEngine

#endif //CLCENGINE2_MOVE_H
