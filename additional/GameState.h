#include <clcengine2/clcengine2.h>
#include <auxTypes/pathmapmarker.h>
#include <auxTypes/gameparameters.h>
#include <QObject>

#ifndef GAMESTATE_H
#define GAMESTATE_H


class GameState: public QObject
{
    Q_OBJECT
    public:

        GameState();
        ~GameState();

        bool hasActiveGame();
        void resetPathmap();
        void makeGame(GameParameters parameters);
        bool isThereACPURival();
        bool noNearestMoves();
        void makeFirstCPUMove();
        bool hasMoveBeenMade();
        bool hasMoves();
        bool isDrawAvailable();

        CLCEngine::CheckerMetaInfo getMetaInfo(CLCEngine::Coordinates at);
        PathMapMarker getPathMapMarker(CLCEngine::Coordinates at);
        CLCEngine::Move getMove(CLCEngine::Coordinates source, CLCEngine::Coordinates destination);

        void findMovesIfRequired();
        void fillPathMap(CLCEngine::Coordinates source);
        void filterNearestMovesWith(CLCEngine::Move move);

        void appendMove(CLCEngine::Move move);
        void applyMoveOnMetaArray(CLCEngine::Move move);

        void commitMove();

        CLCEngine::CheckerColor whoIsPlayer();
        CLCEngine::CheckerColor currentMover();

        CLCEngine::MoveList makeRegularCPUMove();
        void commitCPUMove(CLCEngine::MoveList sequence);
        void flushMetaArray();

        CLCEngine::CheckerColor getPlayerSide();

        void setDrawOfferingState(bool state);
        bool requestCPURivalDrawAgreement();

        int getRivalIndex();
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
