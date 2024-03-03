//
// Created by EgrZver on 04.10.2023.
//

#include "KingMoveFinder.h"

namespace CLCEngine {

    int KingMoveFinder::findDistanceToVictim(
        MetaArray metaArray,
        Coordinates source,
        int dirH,
        int dirV) {

        int shift = 1;

        while (this->longMoveLookingFlag({source.x + shift*dirH, source.y + shift*dirV}, dirH, dirV)) {
            if (
                this->isAFriend(metaArray, source, {source.x + shift*dirH, source.y + shift*dirV}) ||
                this->isMarkedForDeathMeta(metaArray, {source.x + shift*dirH, source.y + shift*dirV})
            ) {
                return -1;
            } else if (this->isAFriend(metaArray, source, {source.x + shift*dirH, source.y + shift*dirV})) {
                return -1;
            } else if (this->isAVictim(metaArray, source, {source.x + shift*dirH, source.y + shift*dirV})) {
                return shift;
            }
            else if (this->isNothingHere(metaArray, {source.x + shift*dirH, source.y + shift*dirV})) {
                shift++;
            }
        }

        return -1;
    }


    bool KingMoveFinder::longMoveLookingFlag(
        Coordinates place,
        int dirH,
        int dirV
    ) {

        if (dirH == (int) Direction::Right && dirV == (int) Direction::Up) {
            return !(place.x >= 7 || place.y >= 7);
        } else if (dirH == (int) Direction::Left && dirV == (int) Direction::Up) {
            return !(place.x <= 0 || place.y >= 7);
        } else if (dirH == (int) Direction::Right && dirV == (int) Direction::Down) {
            return !(place.x >= 7 || place.y <= 0);
        } else if (dirH == (int) Direction::Left && dirV == (int) Direction::Down) {
            return !(place.x <= 0 || place.y <= 0);
        } else {
            return false;
        }
    }

    MoveList KingMoveFinder::getNearestCommonMoves(CLCEngine::MetaArray metaArray, CLCEngine::Coordinates forWhat) {
        MoveList moves;

        for (int dirV = -1; dirV < 2; dirV += 2) {
            for (int dirH = -1; dirH < 2; dirH += 2) {
                bool flag = this->longMoveLookingFlag(forWhat, dirH, dirV);
                int shift = 1;
                while (flag) {
                    if (this->isNothingHere(metaArray, {forWhat.x + shift*dirH, forWhat.y + shift*dirV})) {
                        Move move(
                            forWhat,
                            {forWhat.x + shift*dirH, forWhat.y + shift*dirV},
                            false
                            );
                        moves.append(move);
                        flag = this->longMoveLookingFlag(
                            {forWhat.x + shift*dirH, forWhat.y + shift*dirV},
                            dirH,
                            dirV
                            );
                    } else {
                        flag = false;
                    }
                    shift++;
                }
            }
        }

        return moves;
    }

    MoveList KingMoveFinder::getNearestCapturingMoves(CLCEngine::MetaArray metaArray, CLCEngine::Coordinates forWhat) {
        MoveList moves;

        for (int dirV = -1; dirV < 2; dirV += 2) {
            for (int dirH = -1; dirH < 2; dirH += 2) {

                int victimDistance = this->findDistanceToVictim(metaArray, forWhat, dirH, dirV);
                if (victimDistance != -1) {
                    int additionalShift = 1;
                    bool flag = longMoveLookingFlag(
                        {forWhat.x + victimDistance*dirH, forWhat.y + victimDistance*dirV},
                        dirH,
                        dirV
                        );
                    while (flag) {
                        if (this->isNothingHere(
                            metaArray,
                            {
                                forWhat.x + dirH * (victimDistance + additionalShift),
                                forWhat.y + dirV * (victimDistance + additionalShift)
                                }
                            )) {
                            Move move(
                                forWhat,
                                {
                                    forWhat.x + dirH * (victimDistance + additionalShift),
                                    forWhat.y + dirV * (victimDistance + additionalShift)
                                },
                                false
                                );

                            move.setVictim(metaArray[forWhat.y + dirV * victimDistance][forWhat.x + dirH * victimDistance]);
                            moves.append(move);
                            flag = this->longMoveLookingFlag(
                                {
                                    forWhat.x + dirH * (victimDistance + additionalShift),
                                    forWhat.y + dirV * (victimDistance + additionalShift)
                                },
                                dirH,
                                dirV
                                );
                        } else {
                            flag = false;
                        }
                        additionalShift++;
                    }
                }


            }
        }

        return moves;
    }

} // CLCEngine
