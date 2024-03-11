#include <QString>
#include <QPointF>
#include <clcengine2/checker/CheckerMetaInfo.h>
#include <clcengine2/analyzer/Difficulty.h>

#ifndef AUXULIARY_H
#define AUXULIARY_H

//const CLCEngine::Coordinates INVALID_PLACE = {-1, -1};

class Auxiliary
{


    public:
        //static void auxLoadAssets(QString assetsPath);
        //static int auxGetAssetIndex(CLCEngine::CheckerMetaInfo metaInfo);
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

    //private:

};

#endif // AUXULIARY_H
