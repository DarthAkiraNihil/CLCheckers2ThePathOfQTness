#ifndef GAMESAVEDATA_H
#define GAMESAVEDATA_H
#include <clcengine2/clcengine2.h>
#include "gameType.h"

struct GameSaveData {
    CLCEngine::CheckerMetaInfo boardData[8][8];
    CLCEngine::CheckerColor playerSide;
    CLCEngine::CheckerColor currentMover;
    int checkersCount[2];
    GameType gameType;
    //artype
    CLCEngine::CheckerColor side;
    CLCEngine::Difficulty maxDepth;

};

#endif // GAMESAVEDATA_H
