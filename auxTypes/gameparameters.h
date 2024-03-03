#ifndef GAMEPARAMETERS_H
#define GAMEPARAMETERS_H
#include "clcengine2/analyzer/Difficulty.h"
#include "clcengine2/checker/CheckerMetaInfo.h"
#include "gameType.h"

struct GameParameters
    {
    GameType gameType;
    CLCEngine::Difficulty difficulty;
    CLCEngine::CheckerColor playerSide;
    };

    const GameParameters DEFAULT_PARAMS = {GameType::RivalIsAHuman,
                                           CLCEngine::Difficulty::Human,
                                           CLCEngine::CheckerColor::NoColor};

#endif // GAMEPARAMETERS_H
