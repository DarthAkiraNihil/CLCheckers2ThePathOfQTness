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


const CLCEngine::Coordinates INVALID_PLACE = {-1, -1};

class BoardScene : public QGraphicsScene {
    Q_OBJECT

    public:
        explicit BoardScene(QString assetsPath, QObject *parent = nullptr);
        ~BoardScene();

        void renderContent();
        void constructAGame(GameParameters gameParameters);
        void moveCursor(CLCEngine::Coordinates direction);

        void handleClick();
        void doTheFirstCPUMoveIfRequired();

        GameSaveData exportGameData();
        void importGameData(GameSaveData save);

        void endGame();

        bool isDrawAvailable();
        void setDrawAsOffered();
        bool requestCPURivalDrawAgreement();
        bool isThereACPURival();
        int getRivalIndex();


        QString getSurrenderMessage();
        bool hasActiveGame();

    private:
        void renderBoard();
        void renderPathmap();
        void renderCursor();


        //event
        void mousePressEvent(QGraphicsSceneMouseEvent * event);

        CLCEngine::Board* constructBoard(CLCEngine::CheckerColor playerSide);
        CLCEngine::AbstractComputerRival* constructRival(
            CLCEngine::Difficulty difficulty,
            CLCEngine::CheckerColor side
        );

        void resetPathmap();
        void fillPathMap(CLCEngine::Coordinates source);

        //internal click handle
        void findMovesIfRequired();
        void handleClickNotDestinationSelected();
        void handleClickDestinationSelected();
        void passTurn();

        CLCEngine::Move getMove(CLCEngine::Coordinates source, CLCEngine::Coordinates destination);
        void applyMoveOnMetaArray(CLCEngine::Move move);
        CLCEngine::MoveList filterMovesWith(CLCEngine::Move move);

        void makeASequenceWithDelayOnMeta(CLCEngine::MoveList sequence, int mSecDelay);

        //auxilliary
        void auxLoadAssets(QString assetsPath);
        int auxGetAssetIndex(CLCEngine::CheckerMetaInfo metaInfo);
        CLCEngine::Coordinates auxConvertSceneToBoard(QPointF clickedPlace);
        CLCEngine::CheckerColor auxInvertColor(CLCEngine::CheckerColor color);
        CLCEngine::Coordinates auxConvertBoardToCursor(CLCEngine::Coordinates place);
        CLCEngine::Coordinates auxConvertCursorToBoard();
        int auxConvertDifficultyToIndex(CLCEngine::Difficulty difficulty);

        //freakin' assets
        QPixmap assets[9];

        //game state fields
        int rivalIndex, step;
        CLCEngine::CheckerColor playerSide;
        CLCEngine::Board* gameBoard;
        CLCEngine::AbstractComputerRival* rival;
        CLCEngine::MetaArray activeMetaArray;
        PathMapMarker pathMap[8][8];
        CLCEngine::Coordinates cursor, source, destination;
        bool movesHaveBeenFound, moveHasBeenMade, isGameBegun, drawHasBeenOffered;
        CLCEngine::MoveList currentSequence{}, nearestMoves{};
        CLCEngine::MoveSequencesList nearestSequences{};

        //pixmap res
        std::vector<QGraphicsPixmapItem*> activePixmaps{};
        CLCEngine::DynamicSequence<std::string> gameLog{};
        std::string currentLogLine{};

        void writeLog(std::string filename);
        CLCEngine::Coordinates getNotationSquareCoordinates(CLCEngine::Coordinates place);
        void registerMove(CLCEngine::Move move);
        void registerSequence(CLCEngine::MoveList& sequence);

        QTimer* testt;

    signals:
        void clickHasBeenSomewhere(CLCEngine::Coordinates where);
        void redraw();
        void playerWon();
        void cpuRivalWon(QString message);
        void humanRivalWon();
        void transferStatusBarText(QString text);
        void transferMoveLogLine(QString line);

    private slots:
        void forceRendering();

};

#endif // BOARDSCENE_H
