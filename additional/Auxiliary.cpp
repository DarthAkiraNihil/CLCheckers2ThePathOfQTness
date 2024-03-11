#include "Auxiliary.h"

CLCEngine::Coordinates Auxiliary::convertSceneToBoard(
        QPointF clickedPlace,
        CLCEngine::Coordinates borderConstraintBottom,
        CLCEngine::Coordinates borderConstraintTop,
        CLCEngine::CheckerColor playerSide
    ) {
    if (
        (clickedPlace.x() < borderConstraintBottom.x)   || //4
        (clickedPlace.x() > borderConstraintTop.x)      || //484
        (clickedPlace.y() < borderConstraintBottom.y)   ||
        (clickedPlace.y() > borderConstraintTop.y)
    ) {
        return {-1, -1}; //INVALID
    } else {
        if (
            playerSide == CLCEngine::CheckerColor::White
        ) {
            return {(int) clickedPlace.x() / 60, 7 - (int) clickedPlace.y() / 60};
        } else {
            return {7 -(int) clickedPlace.x() / 60, (int) clickedPlace.y() / 60};
        }
    }
}

CLCEngine::CheckerColor Auxiliary::invertColor(CLCEngine::CheckerColor color) {
    if (color == CLCEngine::CheckerColor::Black) {
        return CLCEngine::CheckerColor::White;
    } else if (color == CLCEngine::CheckerColor::White) {
        return CLCEngine::CheckerColor::Black;
    } else {
        return CLCEngine::CheckerColor::NoColor;
    }
}

CLCEngine::Coordinates Auxiliary::convertBoardToCursor(
    CLCEngine::Coordinates place,
    CLCEngine::CheckerColor playerSide
) {
    if (playerSide == CLCEngine::CheckerColor::White) {
        return {place.x, 7 - place.y};
    } else {
        return {7 - place.x, place.y};
    }
}

CLCEngine::Coordinates Auxiliary::convertCursorToBoard(
        CLCEngine::Coordinates cursor,
        CLCEngine::CheckerColor playerSide
    ) {
    if (playerSide == CLCEngine::CheckerColor::White) {
        return {cursor.x, 7 - cursor.y};
    } else {
        return {7 - cursor.x, cursor.y};
    }
}

int Auxiliary::convertDifficultyToIndex(CLCEngine::Difficulty difficulty) {
    switch (difficulty) {
        case CLCEngine::Difficulty::Dumbass: {
            return 0;
        }
        case CLCEngine::Difficulty::Easy: {
            return 1;

        }
        case CLCEngine::Difficulty::Normal: {
            return 2;
        }
        case CLCEngine::Difficulty::Hard: {
            return 3;
        }
        case CLCEngine::Difficulty::Insane: {
            return 4;
        }
        case CLCEngine::Difficulty::Extreme: {
            return 5;
        }
        default: {
            return 0;
        }
    }
}
