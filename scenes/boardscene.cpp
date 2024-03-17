#include "boardscene.h"
#include <QApplication>
#include <QDebug>
#include <sstream>
#include <random>
#include <QThread>
#include <windows.h>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <additional/Auxiliary.h>
#include "constants.h"
#include <fstream>



BoardScene::BoardScene(AssetLoader* assetLoader, GameState* state, QObject *parent): QGraphicsScene{parent} {
    this->assetLoader = assetLoader;
    this->state = state;
    this->cursor = {0, 0};

    QGraphicsPixmapItem *logo = this->addPixmap(this->assetLoader->getLogo());
    logo->setPos(117, 117);

    this->forcedRenderTimeout = new QTimer;
    QObject::connect(this->forcedRenderTimeout, &QTimer::timeout, this, &BoardScene::forceRendering);
    this->forcedRenderTimeout->start(100);
}

BoardScene::~BoardScene() {
    delete this->forcedRenderTimeout;
}

void BoardScene::forceRendering() {
    QApplication::processEvents();
}

void BoardScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (this->state->hasActiveGame()) {
        CLCEngine::Coordinates clicked = Auxiliary::convertSceneToBoard(
            event->scenePos(),
            {4, 4},
            {484, 484},
            this->state->getPlayerSide()
        );
        if (!(clicked == INVALID_PLACE)) {
            this->cursor = Auxiliary::convertBoardToCursor(
                clicked,
                this->state->getPlayerSide()
            );
            this->renderContent();
        }

        this->handleClick();

        emit this->clickHasBeenSomewhere(clicked);
    }
}

void BoardScene::doTheFirstCPUMoveIfRequired() {
    if (
        this->state->getPlayerSide() != CLCEngine::CheckerColor::White &&
        this->state->isThereACPURival()
    ) {
        emit this->transferStatusBarText("Ход противника");
        this->state->makeFirstCPUMove();
        this->renderContent();
    }
}

/* draw */

void BoardScene::renderContent() {
    this->clear();
    this->activePixmaps.clear();
    this->renderBoard();
    this->renderPathmap();
    this->renderCursor();
    QApplication::processEvents();
}


void BoardScene::renderBoard() {
    qDebug() << "JEB: Drawin' the board";
    this->update(0, 0, 488, 488);
    this->addPixmap(this->assetLoader->getBoard());

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            CLCEngine::CheckerMetaInfo meta = this->state->getMetaInfo({j, i});
            if (meta.color != CLCEngine::CheckerColor::NoColor &&
                meta.type != CLCEngine::CheckerType::NoType) {
                QGraphicsPixmapItem *beingDrawn = this->addPixmap(this->assetLoader->getChecker(meta));
                this->activePixmaps.push_back(beingDrawn);
                CLCEngine::Coordinates insertPlace = Auxiliary::getInsertingPlace(
                    this->state->getPlayerSide(),
                    {j, i},
                    9
                );
                beingDrawn->setPos((float) insertPlace.x, (float) insertPlace.y);
            }
        }
    }

    this->update(0, 0, 488, 488);

}

void BoardScene::renderPathmap() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            PathMapMarker marker = this->state->getPathMapMarker({j, i});
            if (marker != PathMapMarker::Nothing) {
                QGraphicsPixmapItem *beingDrawn = this->addPixmap(this->assetLoader->getPathmapMarker(marker));
                this->activePixmaps.push_back(beingDrawn);
                CLCEngine::Coordinates insertPlace = Auxiliary::getInsertingPlace(
                    this->state->getPlayerSide(),
                    {j, i},
                    4
                );
                beingDrawn->setPos((float) insertPlace.x, (float) insertPlace.y);
            }
        }
    }
}

void BoardScene::renderCursor() {
    QGraphicsPixmapItem *beingDrawn = this->addPixmap(this->assetLoader->getCursor());
    this->activePixmaps.push_back(beingDrawn);
    beingDrawn->setPos((float) (4 + this->cursor.x * 60), (float) (4 + this->cursor.y * 60));
}


void BoardScene::handleClick() {
    this->state->findMovesIfRequired();
    CLCEngine::Coordinates on = Auxiliary::convertCursorToBoard(this->cursor, this->state->getPlayerSide());
    PathMapMarker marker = this->state->getPathMapMarker(on);

    if (marker == PathMapMarker::Destination) {
        this->handleClickDestinationSelected();
    } else {
        this->handleClickNotDestinationSelected();
    }

    if (this->state->hasMoveBeenMade()) {
        this->passTurn();
    }

}


void BoardScene::handleClickNotDestinationSelected() {
    qDebug() << "JEB: Source choosing";
    this->source = Auxiliary::convertCursorToBoard(this->cursor, this->state->getPlayerSide());
    this->state->fillPathMap(this->source);
    this->renderContent();
}


void BoardScene::handleClickDestinationSelected() {
    qDebug() << "JEB: Move";
    this->destination = Auxiliary::convertCursorToBoard(this->cursor, this->state->getPlayerSide());
    CLCEngine::Move moveToMake = this->state->getMove(
        this->source,
        this->destination
    );
    this->state->resetPathmap();
    emit this->registerMove(moveToMake);

    this->state->appendMove(moveToMake);

    this->renderContent();

    this->state->filterNearestMovesWith(moveToMake);

    if (this->state->noNearestMoves()) {
        emit this->commitLogLine();
        this->state->commitMove();
        this->renderContent();
    }

}

void BoardScene::passTurn() {
    if (this->state->hasMoveBeenMade()) {
        if (this->state->isThereACPURival()) {
            emit this->transferStatusBarText("Ход противника");
            QApplication::processEvents();
            if (!this->state->hasMoves()) {
                qDebug() << "JEB: You won";
                emit this->playerWon();
                emit this->transferStatusBarText("Вы победили!");
                return;
            }

            CLCEngine::MoveList theBestSequence = this->state->makeRegularCPUMove();

            emit this->commitLogLine();
            this->makeASequenceWithDelayOnMeta(theBestSequence, 400);
            this->state->commitCPUMove(theBestSequence);

            emit this->registerSequence(theBestSequence);
            emit this->commitLogLine();

            this->renderContent();

            if (!this->state->hasMoves()) {
                //emit this->cpuRivalWon(AppConst::rivalVictoryMessages[this->rivalIndex]);
                emit this->cpuRivalWon("FIX DA SHIT");
                emit this->transferStatusBarText("Вы проиграли");
                return;
            }


            emit this->transferStatusBarText("Ход игрока");

        } else {
            if (this->state->currentMover() == this->state->whoIsPlayer()) {
                emit this->transferStatusBarText("Ход игрока");
            } else {
                emit this->transferStatusBarText("Ход противника");
            }

            if (!this->state->hasMoves()) {
                if (this->state->currentMover() == this->state->whoIsPlayer()) {
                    emit this->humanRivalWon();
                    emit this->transferStatusBarText("Вы проиграли");
                    return;
                } else {
                    emit this->playerWon();
                    emit this->transferStatusBarText("Вы победили!");
                    return;
                }
            }
        }
        this->state->setDrawOfferingState(false);
    }
}



void BoardScene::moveCursor(CLCEngine::Coordinates direction) {
    CLCEngine::Coordinates newCursor = {
        this->cursor.x + direction.x,
        this->cursor.y + direction.y
    };
    if (
        (newCursor.x > -1) &&
        (newCursor.y > -1) &&
        (newCursor.x < 8) &&
        (newCursor.y < 8)
    ) {
        this->cursor = newCursor;
        this->renderContent();
    }
}



void BoardScene::makeASequenceWithDelayOnMeta(CLCEngine::MoveList sequence, int mSecDelay){
    for (int i = 0; i < sequence.getLength(); i++) {
        this->state->applyMoveOnMetaArray(sequence.getElement(i));
        QThread::msleep(mSecDelay);
        this->update(0, 0, 488, 488);
        this->renderContent();
    }
}

GameSaveData BoardScene::exportGameData() {
    /*
    GameSaveData saved;

    CLCEngine::MetaArray savedBoard = this->gameBoard->makeMetaArray();

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            saved.boardData[i][j] = savedBoard[i][j];
        }
    }

    saved.playerSide = this->playerSide;
    saved.currentMover = this->gameBoard->whoMoves();

    CLCEngine::Coordinates painCheckersCount = this->gameBoard->getCheckersCount();
    saved.checkersCount[0] = painCheckersCount.x;
    saved.checkersCount[1] = painCheckersCount.y;

    if (!this->rival) {
        saved.maxDepth = CLCEngine::Difficulty::Human;
        saved.side = Auxiliary::invertColor(this->playerSide);
        saved.gameType = GameType::RivalIsAHuman;
    } else {
        saved.maxDepth = this->rival->getMaxDepth();
        saved.side = this->rival->getSide();
        saved.gameType = GameType::RivalIsACPU;
    }


    for (int i = 0; i < 8; i++) {
        delete savedBoard[i];
    }

    delete [] savedBoard;

    //saved.gameType =

    return saved;

    */
    qDebug() << "NOT IMPLEMENTED YET";
    return {};
}

void BoardScene::importGameData(GameSaveData save) {

//        this->gameBoard = new CLCEngine::Board(
//                              save.playerSide,
//                              save.currentMover,
//                              save.checkersCount
//                          );
//        for (int i = 0; i < 8; i++) {
//            for (int j = 0; j < 8; j++) {
//                this->gameBoard->assignCheckerExternal(save.boardData[i][j]);
//            }
//        }

//        this->rival = (save.gameType == GameType::RivalIsACPU) ?
//                       this->constructRival(
//                           save.maxDepth,
//                           Auxiliary::invertColor(save.playerSide)
//                       ) :
//                       nullptr;
//        this->playerSide = save.playerSide;
//        this->activeMetaArray = this->gameBoard->makeMetaArray();
//        this->isGameBegun = true;
//        this->renderContent();

qDebug() << "NOT IMPLEMENTED YET";

}
