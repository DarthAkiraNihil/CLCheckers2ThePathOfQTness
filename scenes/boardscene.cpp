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

std::random_device rd;
std::seed_seq ssq{rd()};
std::default_random_engine eng {rd()};
//std::mt19937 chancer(rd());
std::uniform_int_distribution<> glist(0, 1000);

BoardScene::BoardScene(QString assetsPath, QObject *parent): QGraphicsScene{parent} {
    this->auxLoadAssets(assetsPath);
    this->playerSide = CLCEngine::CheckerColor::NoColor;
    this->activeMetaArray = nullptr;
    this->gameBoard = nullptr;
    this->rival = nullptr;
    this->cursor = {0, 0};

    this->resetPathmap();
    this->moveHasBeenMade = false;
    this->movesHaveBeenFound = false;

    this->isGameBegun = false;
    this->drawHasBeenOffered = false;
    this->step = 0;
    this->rivalIndex = -1;
    QGraphicsPixmapItem *logo = this->addPixmap(assets[8]);
    logo->setPos(117, 117);
    this->testt = new QTimer;
    QObject::connect(this->testt, &QTimer::timeout, this, &BoardScene::forceRendering);
    this->testt->start(100);
    //this->activePixmaps = new CLCEngine::DynamicSequence<QGraphicsPixmapItem*>;
    //delete logo;

    //setViewportUpdateMode(QGraphicsView::FullViewportUpdate)
}

BoardScene::~BoardScene() {
    delete this->gameBoard;
    delete this->rival;
}

void BoardScene::forceRendering() {
    QApplication::processEvents();
}

void BoardScene::auxLoadAssets(QString assetsPath) {
    for (int i = 0; i < 9; i++) {
        this->assets[i] = QPixmap(assetsPath + "\\" + AppConst::standardAssetsNames[i]);
    }
}

int BoardScene::auxGetAssetIndex(CLCEngine::CheckerMetaInfo metaInfo) {
    int colorDefiner = (metaInfo.color == CLCEngine::CheckerColor::Black) ? 0 : 2,
        typeDefiner = (metaInfo.type == CLCEngine::CheckerType::Common) ? 0: 1;
    return 1 + colorDefiner + typeDefiner;
}





void BoardScene::constructAGame(GameParameters gameParameters) {
    if (this->isGameBegun) this->endGame();
    qDebug() << "JEB: Construction: " << "Side: " << (int) gameParameters.playerSide <<
                " Diff: " << (int) gameParameters.difficulty << " Type: " << (int) gameParameters.gameType;
    this->gameBoard = this->constructBoard(gameParameters.playerSide);
    this->rival = (gameParameters.gameType == GameType::RivalIsACPU) ?
                   this->constructRival(
                       gameParameters.difficulty,
                       Auxiliary::invertColor(gameParameters.playerSide)
                   ) :
                   nullptr;
    this->rivalIndex = Auxiliary::convertDifficultyToIndex(gameParameters.difficulty);
    this->playerSide = gameParameters.playerSide;
    this->activeMetaArray = this->gameBoard->makeMetaArray();
    this->isGameBegun = true;
    emit this->transferStatusBarText("Ход игрока");
}

CLCEngine::Board* BoardScene::constructBoard(CLCEngine::CheckerColor playerSide) {
    return new CLCEngine::Board(playerSide);
}

CLCEngine::AbstractComputerRival* BoardScene::constructRival(CLCEngine::Difficulty difficulty, CLCEngine::CheckerColor side) {
    if (difficulty == CLCEngine::Difficulty::Dumbass) {
        return new CLCEngine::ComputerRivalRandom(side);
    } else {
        return new CLCEngine::ComputerRivalMinimax(difficulty, side);
    }

}



void BoardScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
        //QPointF clicked = event->scenePos();
    if (this->isGameBegun) {
        CLCEngine::Coordinates clicked = Auxiliary::convertSceneToBoard(
            event->scenePos(),
            {4, 4},
            {484, 484},
            this->playerSide
        );
        if (!(clicked == INVALID_PLACE)) {
            this->cursor = Auxiliary::convertBoardToCursor(
                clicked,
                this->playerSide
            );
            this->renderContent();
        }

        this->handleClick();



        emit this->clickHasBeenSomewhere(clicked);
    }
    //QGraphicsScene::mousePressEvent(event);
}

void BoardScene::doTheFirstCPUMoveIfRequired() {
    if (this->playerSide != CLCEngine::CheckerColor::White && this->rival) {
        emit this->transferStatusBarText("Ход противника");
        this->gameBoard->flushLists();
        this->gameBoard->findAvailableMoves();

        CLCEngine::MoveList theBestSequence = this->rival->getNextSequence(*(this->gameBoard));
        //this->makeASequenceWithDelayOnMeta(this->activeMetaArray, theBestSequence, 800);
        this->gameBoard->makeASequence(theBestSequence);
        this->gameBoard->flushLists();
        this->gameBoard->passTurn();

        for (int i = 0; i < 8; i++) {
            delete [] this->activeMetaArray[i];
        }
        delete [] this->activeMetaArray;

        this->activeMetaArray = this->gameBoard->makeMetaArray();

        this->renderContent();

    }
}

bool BoardScene::hasActiveGame() {
    return this->isGameBegun;
}

/* draw */

void BoardScene::renderContent() {
    this->clear();
    this->activePixmaps.clear();
    this->renderBoard();
    //QApplication::processEvents();
    //qDebug() << "JEB: Event processed";
    this->renderPathmap();
    //QApplication::processEvents();
    //qDebug() << "JEB: Event processed"; //OH IM FUCKING CUMMING
    this->renderCursor();
    QApplication::processEvents();
    //qDebug() << "JEB: Event processed";
    //qDebug() << "JEB: Finished rendering";

}


void BoardScene::renderBoard() {
    //CLCEngine::MetaArray metaArray = this->gameBoard->makeMetaArray();
    //QImage asset; asset.load("assets/board.png");
    //asset.setOffset({20, 20})
    qDebug() << "JEB: Drawin' the board";
    this->update(0, 0, 488, 488); // todo make it painter
    this->addPixmap(this->assets[0]);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (this->activeMetaArray[i][j].color != CLCEngine::CheckerColor::NoColor &&
                this->activeMetaArray[i][j].type != CLCEngine::CheckerType::NoType) {
                QGraphicsPixmapItem *beingDrawn = this->addPixmap(this->assets[this->auxGetAssetIndex(this->activeMetaArray[i][j])]);
                this->activePixmaps.push_back(beingDrawn);
                //sf::Sprite beingDrawn(this->assets[this->getAssetIndex(metaArray[i][j])]);
                if (this->playerSide == CLCEngine::CheckerColor::Black) {
                    beingDrawn->setPos((float) (9 + (7 - j) * 60), (float) (9 + i * 60));
                } else {
                    beingDrawn->setPos((float) (9 + j * 60), (float) (9 + (7 - i) * 60));
                }

            }
        }
    }

    this->update(0, 0, 488, 488);

}

void BoardScene::renderPathmap() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (this->pathMap[i][j] != PathMapMarker::Nothing) {

                QGraphicsPixmapItem *beingDrawn = this->addPixmap(
                    this->assets[
                        (this->pathMap[i][j] == PathMapMarker::Destination) ? 6 : 7
                    ]
                );
                this->activePixmaps.push_back(beingDrawn);
                //sf::Sprite beingDrawn(this->assets[this->getAssetIndex(metaArray[i][j])]);
                // TODO
                if (this->playerSide == CLCEngine::CheckerColor::Black) {
                    beingDrawn->setPos((float) (4 + (7 - j) * 60), (float) (4 + i * 60));
                } else {
                    beingDrawn->setPos((float) (4 + j * 60), (float) (4 + (7 - i) * 60));
                }

            }
        }
    }
}

void BoardScene::renderCursor() {
    QGraphicsPixmapItem *beingDrawn = this->addPixmap(this->assets[5]);
    this->activePixmaps.push_back(beingDrawn);
    beingDrawn->setPos((float) (4 + this->cursor.x * 60), (float) (4 + this->cursor.y * 60));
}


void BoardScene::handleClick() {
    this->findMovesIfRequired();
    CLCEngine::Coordinates on = Auxiliary::convertCursorToBoard(this->cursor, this->playerSide);
    PathMapMarker marker = this->pathMap[on.y][on.x];
    if (marker == PathMapMarker::Destination) {
        this->handleClickDestinationSelected();
    } else {
        this->handleClickNotDestinationSelected();
    }
    //this->update(this->sceneRect());

    if (this->moveHasBeenMade) {
        this->passTurn();
    }

}

void BoardScene::findMovesIfRequired() {

    if (!this->movesHaveBeenFound) {

        this->gameBoard->flushLists();
        this->gameBoard->findAvailableMoves();
        this->nearestSequences = this->gameBoard->getAvailableSequences();
        this->step = 0;
        for (int i = 0; i < this->nearestSequences.getLength(); i++) {
            this->nearestMoves.append(this->nearestSequences.getElement(i).getElement(0));
        }
        this->movesHaveBeenFound = true;
    }
}

void BoardScene::handleClickNotDestinationSelected() {
    qDebug() << "JEB: Source choosing";
    this->source = Auxiliary::convertCursorToBoard(this->cursor, this->playerSide);
    this->fillPathMap(this->source);
    this->renderContent();
}


void BoardScene::handleClickDestinationSelected() {
    qDebug() << "JEB: Move";
    this->destination = Auxiliary::convertCursorToBoard(this->cursor, this->playerSide);
    CLCEngine::Move moveToMake = this->getMove(
        this->source,
        this->destination
    );
    this->resetPathmap();
    this->registerMove(moveToMake);

    this->currentSequence.append(moveToMake);
    this->applyMoveOnMetaArray(moveToMake);
    this->step++;
    this->renderContent();
    this->nearestMoves = this->filterMovesWith(moveToMake);

    if (this->nearestMoves.getLength() == 0) {
        this->gameLog.append(this->currentLogLine);
        qDebug() << "JEB: Made move: " << this->currentLogLine.c_str();
        //QString conv = this->currentLogLine
        emit this->transferMoveLogLine(this->currentLogLine.c_str());
        this->moveHasBeenMade = true;
        this->movesHaveBeenFound = false;
        this->gameBoard->flushLists();
        this->gameBoard->makeASequence(this->currentSequence);
        this->currentSequence.clear();
        this->resetPathmap();
        this->gameBoard->passTurn();
        this->step = 0;
        for (int i = 0; i < 8; i++) {
            delete [] this->activeMetaArray[i];
        }
        delete [] this->activeMetaArray;
        this->currentLogLine.clear();

        this->activeMetaArray = this->gameBoard->makeMetaArray();
        this->renderContent();
    }

}
void BoardScene::resetPathmap() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            this->pathMap[i][j] = PathMapMarker::Nothing;
        }
    }
}
\
void BoardScene::fillPathMap(CLCEngine::Coordinates source) {
    this->resetPathmap();
        for (int i = 0; i < this->nearestMoves.getLength(); i++) {
            CLCEngine::Move move = this->nearestMoves.getElement(i);
            CLCEngine::Coordinates src = move.getSource(), dst = move.getDestination();
            if (src == source) {
            this->pathMap[dst.y][dst.x] = PathMapMarker::Destination;
            CLCEngine::CheckerMetaInfo victim = move.getVictimMetaInfo();
            if (!(victim == CLCEngine::NO_VICTIM)) {
                this->pathMap[victim.coordinates.y][victim.coordinates.x] = PathMapMarker::Victim;
            }
        }
    }
}
CLCEngine::Move BoardScene::getMove(CLCEngine::Coordinates source, CLCEngine::Coordinates destination) {
    for (int i = 0; i < this->nearestMoves.getLength(); i++) {
        CLCEngine::Move move = this->nearestMoves.getElement(i);
        if (move.getSource() == source && move.getDestination() == destination) {
            return move;
        }
    }
    return {};
}

void BoardScene::applyMoveOnMetaArray(CLCEngine::Move move) {
    CLCEngine::Coordinates src = move.getSource(), dst = move.getDestination();
    this->activeMetaArray[dst.y][dst.x] = this->activeMetaArray[src.y][src.x];

    this->activeMetaArray[src.y][src.x].color = CLCEngine::CheckerColor::NoColor;
    this->activeMetaArray[src.y][src.x].type = CLCEngine::CheckerType::NoType;
    this->activeMetaArray[src.y][src.x].markedForDeath = false;

    this->activeMetaArray[dst.y][dst.x].coordinates = dst;
    CLCEngine::CheckerMetaInfo victim = move.getVictimMetaInfo();
    if (!(victim == CLCEngine::NO_VICTIM)) {
        this->activeMetaArray[victim.coordinates.y][victim.coordinates.x].markedForDeath = true;
    }
}

CLCEngine::MoveList BoardScene::filterMovesWith(CLCEngine::Move move) {
    CLCEngine::MoveList toReturn;
    for (int i = 0; i < this->nearestSequences.getLength(); i++) {
        if (this->nearestSequences.getElement(i).getLength() > this->step) {
            CLCEngine::Move extracted = this->nearestSequences.getElement(i).getElement(this->step - 1);
            //CLCEngine::Move sec = this->nearestSequences.getElement(i).getElement(this->step);
            if (
                    extracted.getDestination() == move.getDestination() &&
                    extracted.getSource() == move.getSource() &&
                    extracted.getVictimMetaInfo() == move.getVictimMetaInfo() &&
                    extracted.isSpecial() == move.isSpecial()

                    ) {
                toReturn.append(this->nearestSequences.getElement(i).getElement(this->step));
            }
        }
    }
    return toReturn;
}

void BoardScene::passTurn() {
    if (this->moveHasBeenMade) {
        if (this->rival) {
            emit this->transferStatusBarText("Ход противника");
            QApplication::processEvents();
            this->gameBoard->findAvailableMoves();

            if (this->gameBoard->noMovesNow()) {
                qDebug() << "JEB: You won";
                emit this->playerWon();
                emit this->transferStatusBarText("Вы победили!");
                return;
            }

            this->gameBoard->flushLists();

            CLCEngine::MoveList theBestSequence = this->rival->getNextSequence(*(this->gameBoard));

            if (theBestSequence.getLength() == 0) {
                CLCEngine::MoveSequencesList nearestSequences = this->gameBoard->getAvailableSequences();
                std::uniform_int_distribution<> emergency(0, nearestSequences.getLength() - 1);
                theBestSequence = nearestSequences.getElement(emergency(eng));

            }

            this->registerSequence(theBestSequence);
            qDebug() << "JEB: Made CPU sequence: " << this->currentLogLine.c_str();
            this->gameLog.append(this->currentLogLine);
            emit this->transferMoveLogLine(this->currentLogLine.c_str());
            this->currentLogLine.clear();

            this->makeASequenceWithDelayOnMeta(theBestSequence, 400);
            this->gameBoard->makeASequence(theBestSequence);
            this->gameBoard->flushLists();
            this->gameBoard->passTurn(); //send delete > controller

            for (int i = 0; i < 8; i++) {
                delete [] this->activeMetaArray[i];
            }
            delete [] this->activeMetaArray;

            this->activeMetaArray = this->gameBoard->makeMetaArray();

            this->renderContent();
            //QApplication::pro
            this->moveHasBeenMade = false;

            this->gameBoard->findAvailableMoves();

            if (this->gameBoard->noMovesNow()) {
                qDebug() << "JEB: You lost";
                qDebug() << "JEB: Rival index:" << this->rivalIndex;

                emit this->cpuRivalWon(AppConst::rivalVictoryMessages[this->rivalIndex]);
                emit this->transferStatusBarText("Вы проиграли");
                return;
            }

            this->gameBoard->flushLists();
            emit this->transferStatusBarText("Ход игрока");

        } else {
            for (int i = 0; i < 8; i++) {
                delete [] this->activeMetaArray[i];
            }
            delete [] this->activeMetaArray;

            this->activeMetaArray = this->gameBoard->makeMetaArray();
            this->moveHasBeenMade = false;

            this->gameBoard->findAvailableMoves();

            if (this->gameBoard->whoMoves() == this->gameBoard->whoIsPlayer()) {
                emit this->transferStatusBarText("Ход игрока");
            } else {
                emit this->transferStatusBarText("Ход противника");
            }

            if (this->gameBoard->noMovesNow()) {
                if (this->gameBoard->whoIsPlayer() == this->gameBoard->whoMoves()) {
                    emit this->humanRivalWon();
                    emit this->transferStatusBarText("Вы проиграли");
                    return;
                } else {
                    emit this->playerWon();
                    emit this->transferStatusBarText("Вы победили!");
                    return;
                }
            }

            this->gameBoard->flushLists();
        }
        this->drawHasBeenOffered = false;
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
        this->applyMoveOnMetaArray(sequence.getElement(i));
        QThread::msleep(mSecDelay);
        this->update(0, 0, 488, 488);
        this->renderContent();
        //Sleep(mSecDelay);


    }
   }


GameSaveData BoardScene::exportGameData() {
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
}

void BoardScene::importGameData(GameSaveData save) {

        this->gameBoard = new CLCEngine::Board(
                              save.playerSide,
                              save.currentMover,
                              save.checkersCount
                          );
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                this->gameBoard->assignCheckerExternal(save.boardData[i][j]);
            }
        }

        this->rival = (save.gameType == GameType::RivalIsACPU) ?
                       this->constructRival(
                           save.maxDepth,
                           Auxiliary::invertColor(save.playerSide)
                       ) :
                       nullptr;
        this->playerSide = save.playerSide;
        this->activeMetaArray = this->gameBoard->makeMetaArray();
        this->isGameBegun = true;
        this->renderContent();



}

void BoardScene::endGame() {
    this->playerSide = CLCEngine::CheckerColor::NoColor;
    for (int i = 0; i < 8; i++) {
        delete [] this->activeMetaArray[i];
    }
    delete [] this->activeMetaArray;

    delete this->gameBoard;

    delete this->rival;

    this->activeMetaArray = nullptr;
    this->gameBoard = nullptr;
    this->rival = nullptr;
    this->cursor = {0, 0};

    this->resetPathmap();
    this->moveHasBeenMade = false;
    this->movesHaveBeenFound = false;
    this->step = 0;
    this->isGameBegun = false;
    this->drawHasBeenOffered = false;

    this->writeLog("test.log");
}

bool BoardScene::isThereACPURival() {
    return this->rival;
}

bool BoardScene::requestCPURivalDrawAgreement() {
    return glist(eng) < AppConst::cpuDrawChances[this->rivalIndex];
}

int BoardScene::getRivalIndex() {
    return this->rivalIndex;
}

bool BoardScene::isDrawAvailable() {
    return !this->drawHasBeenOffered;
}

void BoardScene::setDrawAsOffered() {
    this->drawHasBeenOffered = true;
}

void BoardScene::writeLog(std::string filename) {
    std::fstream out;
    out.open(filename, std::ios::out);
    for (int i = 0; i < this->gameLog.getLength(); i++) {
        out << this->gameLog.getElement(i) << "\n";
    }
    out.close();
    this->gameLog.clear();
}

CLCEngine::Coordinates BoardScene::getNotationSquareCoordinates(CLCEngine::Coordinates place) {
    return {61 + place.x, 49 + place.y};
}

void BoardScene::registerMove(CLCEngine::Move move) {
    CLCEngine::Coordinates
       src = move.getSource(),
       dst = move.getDestination();
    if (this->currentLogLine.empty()) {
        this->currentLogLine += 'a' + src.x;
        this->currentLogLine += '1' + src.y;
        this->currentLogLine += (move.getVictimMetaInfo() == CLCEngine::NO_VICTIM) ? '-' : ':';
        this->currentLogLine += 'a' + dst.x;
        this->currentLogLine += '1' + dst.y;
    } else {
        this->currentLogLine += (move.getVictimMetaInfo() == CLCEngine::NO_VICTIM) ? '-' : ':';
        this->currentLogLine += 'a' + dst.x;
        this->currentLogLine += '1' + dst.y;
    }
}

void BoardScene::registerSequence(CLCEngine::MoveList& sequence) {
    for (int i = 0; i < sequence.getLength(); i++) {
        this->registerMove(sequence.getElement(i));
    }
}


