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

    //this->playerSide = CLCEngine::CheckerColor::NoColor;
    //this->activeMetaArray = nullptr;
    //this->gameBoard = nullptr;
    //this->rival = nullptr;
    this->cursor = {0, 0};

    //this->resetPathmap();
    //this->moveHasBeenMade = false;
    //this->movesHaveBeenFound = false;

    //this->isGameBegun = false;
    //this->drawHasBeenOffered = false;
    //this->step = 0;
    //this->rivalIndex = -1;
    QGraphicsPixmapItem *logo = this->addPixmap(this->assetLoader->getLogo());
    logo->setPos(117, 117);


    this->forcedRenderTimeout = new QTimer;
    QObject::connect(this->forcedRenderTimeout, &QTimer::timeout, this, &BoardScene::forceRendering);
    this->forcedRenderTimeout->start(100);
}

BoardScene::~BoardScene() {
    //delete this->gameBoard;
    //delete this->rival;
}

void BoardScene::forceRendering() {
    QApplication::processEvents();
}

void BoardScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
        //QPointF clicked = event->scenePos();
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
                //if (this->playerSide == CLCEngine::CheckerColor::Black) {
                //    beingDrawn->setPos((float) (9 + (7 - j) * 60), (float) (9 + i * 60));
                //} else {
                //    beingDrawn->setPos((float) (9 + j * 60), (float) (9 + (7 - i) * 60));
                //}

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
    //this->update(this->sceneRect());

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
        //this->gameLog.append(this->currentLogLine);
        //qDebug() << "JEB: Made move: " << this->currentLogLine.c_str();
        //QString conv = this->currentLogLine
        //emit this->transferMoveLogLine(this->currentLogLine.c_str());
        this->state->commitMove();
        //this->currentLogLine.clear();
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
            //this->gameBoard->findAvailableMoves();
//
            //if (this->gameBoard->noMovesNow()) {
            //    qDebug() << "JEB: You won";
            //    emit this->playerWon();
             //   emit this->transferStatusBarText("Вы победили!");
            //    return;
            //}
//
            //this->gameBoard->flushLists();

            CLCEngine::MoveList theBestSequence = this->state->makeRegularCPUMove();


            emit this->commitLogLine();
//            emit this->transferMoveLogLine(this->currentLogLine.c_str());
//            this->currentLogLine.clear();

            this->makeASequenceWithDelayOnMeta(theBestSequence, 400);
            this->state->commitCPUMove(theBestSequence);

//            this->gameBoard->makeASequence(theBestSequence);
//            this->gameBoard->flushLists();
//            this->gameBoard->passTurn(); //send delete > controller

//            for (int i = 0; i < 8; i++) {
//                delete [] this->activeMetaArray[i];
//            }
//            delete [] this->activeMetaArray;

//            this->activeMetaArray = this->gameBoard->makeMetaArray();

            emit this->registerSequence(theBestSequence);
            emit this->commitLogLine();
           // this->registerSequence(theBestSequence);
            //qDebug() << "JEB: Made CPU sequence: " << this->currentLogLine.c_str();
            //this->gameLog.append(this->currentLogLine);

            this->renderContent();
            //QApplication::pro
//            this->moveHasBeenMade = false;

//            this->gameBoard->findAvailableMoves();
            if (!this->state->hasMoves()) {

                //emit this->cpuRivalWon(AppConst::rivalVictoryMessages[this->rivalIndex]);
                emit this->cpuRivalWon("FIX DA SHIT");
                emit this->transferStatusBarText("Вы проиграли");
                return;
            }


            emit this->transferStatusBarText("Ход игрока");

        } else {
//            for (int i = 0; i < 8; i++) {
//                delete [] this->activeMetaArray[i];
//            }
//            delete [] this->activeMetaArray;

//            this->activeMetaArray = this->gameBoard->makeMetaArray();
//            this->moveHasBeenMade = false;

            //this->gameBoard->findAvailableMoves();

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

            //if (this->gameBoard->noMovesNow()) {

            //}

            //this->gameBoard->flushLists();
        }
        this->state->setDrawOfferingState(false);
    }
}



void BoardScene::moveCursor(CLCEngine::Coordinates direction) {
    CLCEngine::Coordinates newCursor = {
        this->cursor.x + direction.x,
        this->cursor.y + direction.y
    };
    //qDebug() << newCursor.x << " " << newCursor.y;
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
        //Sleep(mSecDelay);


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

//void BoardScene::endGame() {

    //this->writeLog("test.log");
//}

//bool BoardScene::requestCPURivalDrawAgreement() {
//    return glist(eng) < AppConst::cpuDrawChances[this->rivalIndex];
//}

//int BoardScene::getRivalIndex() {
//    return this->rivalIndex;
//}

//bool BoardScene::isDrawAvailable() {
//    return !this->drawHasBeenOffered;
//}

//void BoardScene::setDrawAsOffered() {
//    this->drawHasBeenOffered = true;
//}



//CLCEngine::Coordinates BoardScene::getNotationSquareCoordinates(CLCEngine::Coordinates place) {
//    return {61 + place.x, 49 + place.y};
//}
