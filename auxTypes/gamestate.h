#ifndef GAMESTATE_H
#define GAMESTATE_H

enum class GameState {
    MovesHaveNotBeenFound,
    WaitingForSelection,
    SourceSelected,
    WaitingForSelectionUnfinishedMove,

};

#endif // GAMESTATE_H
