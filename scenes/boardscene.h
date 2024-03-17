#ifndef BOARDSCENE_H
#define BOARDSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QResizeEvent>
#include <QMessageBox>
#include <QPaintEvent>
#include <QTimer>
#include <clcengine2/clcengine2.h>
#include <auxTypes/gamesavedata.h>
#include <auxTypes/gameparameters.h>
#include <auxTypes/pathmapmarker.h>
#include <vector>
#include <additional/AssetLoader.h>
#include <additional/GameState.h>


const CLCEngine::Coordinates INVALID_PLACE = {-1, -1};

class BoardScene : public QGraphicsScene {
    Q_OBJECT

    public:
        BoardScene(AssetLoader* assetLoader, GameState* state, QObject *parent = nullptr);
        ~BoardScene();

        void renderContent();
        void constructAGame(GameParameters gameParameters);
        void moveCursor(CLCEngine::Coordinates direction);

        void handleClick();
        void doTheFirstCPUMoveIfRequired();

        GameSaveData exportGameData();
        void importGameData(GameSaveData save);

        //void endGame();

        bool isDrawAvailable();
        void setDrawAsOffered();
        bool requestCPURivalDrawAgreement();
        //bool isThereACPURival();
        int getRivalIndex();


        QString getSurrenderMessage();
        //GameState* getStateReference();
        //bool hasActiveGame();

    private:
        void renderBoard();
        void renderPathmap();
        void renderCursor();


        //event
        void mousePressEvent(QGraphicsSceneMouseEvent * event);

        //CLCEngine::Board* constructBoard(CLCEngine::CheckerColor playerSide);
        //CLCEngine::AbstractComputerRival* constructRival(
        //    CLCEngine::Difficulty difficulty,
        //    CLCEngine::CheckerColor side
        //);

        //void resetPathmap();
        //void fillPathMap(CLCEngine::Coordinates source);

        //internal click handle
        //void findMovesIfRequired();
        void handleClickNotDestinationSelected();
        void handleClickDestinationSelected();
        void passTurn();

        //CLCEngine::Move getMove(CLCEngine::Coordinates source, CLCEngine::Coordinates destination);
        //void applyMoveOnMetaArray(CLCEngine::Move move);
        CLCEngine::MoveList filterMovesWith(CLCEngine::Move move);

        void makeASequenceWithDelayOnMeta(CLCEngine::MoveList sequence, int mSecDelay);

        AssetLoader* assetLoader;
        GameState* state;

        //game state fields
        //int rivalIndex, step;
        //CLCEngine::CheckerColor playerSide;
        //CLCEngine::Board* gameBoard;
        //CLCEngine::AbstractComputerRival* rival;
        //CLCEngine::MetaArray activeMetaArray;
        //PathMapMarker pathMap[8][8];
        CLCEngine::Coordinates cursor, source, destination;
        //bool movesHaveBeenFound, moveHasBeenMade, isGameBegun, drawHasBeenOffered;
        //CLCEngine::MoveList currentSequence{}, nearestMoves{};
        //CLCEngine::MoveSequencesList nearestSequences{};

        //pixmap res
        std::vector<QGraphicsPixmapItem*> activePixmaps{};

//        CLCEngine::DynamicSequence<std::string> gameLog{};
//        std::string currentLogLine{};

//        void writeLog(std::string filename);
//        CLCEngine::Coordinates getNotationSquareCoordinates(CLCEngine::Coordinates place);
//        void registerMove(CLCEngine::Move move);
//        void registerSequence(CLCEngine::MoveList& sequence);

        QTimer* forcedRenderTimeout;

    signals:
        void clickHasBeenSomewhere(CLCEngine::Coordinates where);
        void redraw();
        void playerWon();
        void cpuRivalWon(QString message);
        void humanRivalWon();
        void transferStatusBarText(QString text);
        void transferMoveLogLine(QString line);

        void registerMove(CLCEngine::Move move);
        void registerSequence(CLCEngine::MoveList sequence);
        void commitLogLine();

    private slots:
        void forceRendering();

};

#endif // BOARDSCENE_H
