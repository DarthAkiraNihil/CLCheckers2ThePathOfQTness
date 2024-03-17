#include <clcengine2/clcengine2.h>
#include <auxTypes/pathmapmarker.h>
#include <auxTypes/gameparameters.h>
#include <QObject>
#include <auxTypes/gamesavedata.h>

#ifndef GAMESTATE_H
#define GAMESTATE_H


class GameState: public QObject
{
    Q_OBJECT
    public:

        GameState();
        ~GameState();

        void resetPathmap();
        void makeGame(GameParameters parameters);
        void makeFirstCPUMove();

        bool hasMoveBeenMade();
        bool hasMoves();
        bool isDrawAvailable();
        bool isThereACPURival();
        bool noNearestMoves();
        bool hasActiveGame();

        CLCEngine::CheckerMetaInfo getMetaInfo(CLCEngine::Coordinates at);
        PathMapMarker getPathMapMarker(CLCEngine::Coordinates at);
        CLCEngine::Move getMove(CLCEngine::Coordinates source, CLCEngine::Coordinates destination);
        CLCEngine::CheckerColor getPlayerSide();

        CLCEngine::CheckerColor whoIsPlayer();
        CLCEngine::CheckerColor currentMover();

        int getRivalIndex();

        void findMovesIfRequired();
        void fillPathMap(CLCEngine::Coordinates source);
        void filterNearestMovesWith(CLCEngine::Move move);

        void appendMove(CLCEngine::Move move);
        void applyMoveOnMetaArray(CLCEngine::Move move);

        void commitMove();

        CLCEngine::MoveList makeRegularCPUMove();
        void commitCPUMove(CLCEngine::MoveList sequence);
        void flushMetaArray();

        void setDrawOfferingState(bool state);
        bool requestCPURivalDrawAgreement();

        GameSaveData exportGameData();
        void importGameData(GameSaveData save);

    private:
        void setAsNull();
        CLCEngine::MoveList filter(CLCEngine::Move move);

        CLCEngine::Board* makeBoard(CLCEngine::CheckerColor playerSide);
        CLCEngine::AbstractComputerRival* makeRival(
            CLCEngine::Difficulty difficulty,
            CLCEngine::CheckerColor side
        );

        int rivalIndex, step;
        CLCEngine::CheckerColor playerSide;
        CLCEngine::Board* gameBoard;
        CLCEngine::AbstractComputerRival* rival;
        CLCEngine::MetaArray activeMetaArray;
        PathMapMarker pathMap[8][8];
        bool movesHaveBeenFound, moveHasBeenMade, isGameBegun, drawHasBeenOffered;
        CLCEngine::MoveList currentSequence{}, nearestMoves{};
        CLCEngine::MoveSequencesList nearestSequences{};

    public slots:
        void endGame();
};

#endif // GAMESTATE_H
