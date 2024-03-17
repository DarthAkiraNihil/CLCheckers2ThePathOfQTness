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

        //GameSaveData exportGameData();
        //void importGameData(GameSaveData save);

        bool isDrawAvailable();
        void setDrawAsOffered();
        bool requestCPURivalDrawAgreement();
        QString getSurrenderMessage();
        //void update

    private:
        void renderBoard();
        void renderPathmap();
        void renderCursor();

        void mousePressEvent(QGraphicsSceneMouseEvent * event);

        void handleClickNotDestinationSelected();
        void handleClickDestinationSelected();
        void passTurn();

        void makeASequenceWithDelayOnMeta(CLCEngine::MoveList sequence, int mSecDelay);

        AssetLoader* assetLoader;
        GameState* state;
        CLCEngine::Coordinates cursor, source, destination;
        std::vector<QGraphicsPixmapItem*> activePixmaps{};
        QTimer* forcedRenderTimeout;

    signals:
        void clickHasBeenSomewhere(CLCEngine::Coordinates where);
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
