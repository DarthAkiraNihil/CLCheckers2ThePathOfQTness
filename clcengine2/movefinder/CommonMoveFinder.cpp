//
// Created by EgrZver on 04.10.2023.
//

#include "CommonMoveFinder.h"

namespace CLCEngine {

    MoveList CommonMoveFinder::getNearestCommonMoves(CLCEngine::MetaArray metaArray, CLCEngine::Coordinates forWhat) {
        MoveList moves;

        if (
            (metaArray[forWhat.y][forWhat.x].color == CheckerColor::White) &&
            (forWhat.y < 7)
            ) {
            for (int dirH = -1; dirH < 2; dirH += 2) {
                if (
                    (dirH == (int) Direction::Right) ? //Right = 1
                    forWhat.x < 7 :
                    forWhat.x > 0
                    ) {
                    if (this->isNothingHere(metaArray, {forWhat.x + dirH, forWhat.y + 1})) {
                        Move move(
                            forWhat,
                            {forWhat.x + dirH, forWhat.y + 1},
                            (forWhat.y == 6)
                        );
                        moves.append(move);
                    }
                }
                //UP
            }
        } else if (
            (metaArray[forWhat.y][forWhat.x].color == CheckerColor::Black) &&
            (forWhat.y > 1)
            ) {
            //DOWN
            for (int dirH = -1; dirH < 2; dirH += 2) {
                if (
                    (dirH == (int) Direction::Right) ?
                    forWhat.x < 7 :
                    forWhat.x > 0
                    ) {
                    if (this->isNothingHere(metaArray, {forWhat.x + dirH, forWhat.y - 1})) {
                        Move move(
                            forWhat,
                            {forWhat.x + dirH, forWhat.y - 1},
                            (forWhat.y == 1)
                        );
                        moves.append(move);
                    }
                }
            }
        }

        return moves;
    }

    MoveList CommonMoveFinder::getNearestCapturingMoves(CLCEngine::MetaArray metaArray,
                                                        CLCEngine::Coordinates forWhat) {
        MoveList moves;

        for (int dirV = -1; dirV < 2; dirV += 2) {
            for (int dirH = -1; dirH < 2; dirH += 2) {
                if ((dirV == (int) Direction::Down) ? forWhat.y > 1 : forWhat.y < 6) {
                    if ((dirH == (int) Direction::Left) ? forWhat.x > 1 : forWhat.x < 6) {
                        if(
                            this->isNothingHere(metaArray, {forWhat.x + 2*dirH, forWhat.y + 2*dirV}) &&
                            this->isAVictim(metaArray, forWhat, {forWhat.x + dirH, forWhat.y + dirV}) &&
                            !(this->isMarkedForDeathMeta(metaArray, {forWhat.x + dirH, forWhat.y + dirV}))
                            ) {

                            Move move(
                                forWhat,
                                {forWhat.x + 2*dirH, forWhat.y + 2*dirV},
                                ((forWhat.y == 5) && (metaArray[forWhat.y][forWhat.x].color == CheckerColor::White) && (dirV == (int) Direction::Up)) ||
                                ((forWhat.y == 2) && (metaArray[forWhat.y][forWhat.x].color == CheckerColor::Black)  && (dirV == (int) Direction::Down))
                                );
                            move.setVictim(metaArray[forWhat.y + dirV][forWhat.x + dirH]);
                            moves.append(move);
                        }
                    }
                }
            }
        }

        return moves;
    }
} // CLCEngine