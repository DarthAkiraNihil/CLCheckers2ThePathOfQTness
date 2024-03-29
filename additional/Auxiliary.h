#include <QString>
#include <QPointF>
#include <clcengine2/checker/CheckerMetaInfo.h>
#include <clcengine2/analyzer/Difficulty.h>

#ifndef AUXULIARY_H
#define AUXULIARY_H

class Auxiliary
{


    public:
        static CLCEngine::Coordinates convertSceneToBoard(
            QPointF clickedPlace,
            CLCEngine::Coordinates borderConstraintBottom,
            CLCEngine::Coordinates borderConstraintTop,
            CLCEngine::CheckerColor playerSide
        );
        static CLCEngine::CheckerColor invertColor(CLCEngine::CheckerColor color);
        static CLCEngine::Coordinates convertBoardToCursor(
            CLCEngine::Coordinates place,
            CLCEngine::CheckerColor playerSide
        );
        static CLCEngine::Coordinates convertCursorToBoard(
            CLCEngine::Coordinates cursor,
            CLCEngine::CheckerColor playerSide
        );
        static int convertDifficultyToIndex(CLCEngine::Difficulty difficulty);
        static CLCEngine::Difficulty getDifficultyByIndex(int index);
        static CLCEngine::CheckerColor generatePlayerSide();
        static CLCEngine::Coordinates getInsertingPlace(CLCEngine::CheckerColor color, CLCEngine::Coordinates forPlace, int offset);

};

#endif // AUXULIARY_H
