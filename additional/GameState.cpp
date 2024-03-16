#include "GameState.h"
#include <additional/Auxiliary.h>
#include <random>
#include <constants.h>
#include <mainwindow.h>

std::random_device rd;
std::seed_seq ssq{rd()};
std::default_random_engine eng {rd()};
//std::mt19937 chancer(rd());
std::uniform_int_distribution<> glist(0, 1000);

GameState::GameState()
{
    this->setAsNull();
}

GameState::~GameState() {
    delete this->gameBoard;
    delete this->rival;
}

void GameState::makeFirstCPUMove() {
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
}

CLCEngine::CheckerColor GameState::getPlayerSide() {
    return this->playerSide;
}

CLCEngine::CheckerMetaInfo GameState::getMetaInfo(CLCEngine::Coordinates at) {
    return this->activeMetaArray[at.y][at.x];
}

PathMapMarker GameState::getPathMapMarker(CLCEngine::Coordinates at) {
    return this->pathMap[at.y][at.x];
}

CLCEngine::Move GameState::getMove(CLCEngine::Coordinates source, CLCEngine::Coordinates destination) {
    for (int i = 0; i < this->nearestMoves.getLength(); i++) {
        CLCEngine::Move move = this->nearestMoves.getElement(i);
        if (move.getSource() == source && move.getDestination() == destination) {
            return move;
        }
    }
    return {};
}

bool GameState::isThereACPURival() {
    return this->rival;
}

void GameState::setAsNull() {
    this->playerSide = CLCEngine::CheckerColor::NoColor;
    this->activeMetaArray = nullptr;
    this->gameBoard = nullptr;
    this->rival = nullptr;

    this->resetPathmap();

    this->moveHasBeenMade = false;
    this->movesHaveBeenFound = false;
    this->isGameBegun = false;
    this->drawHasBeenOffered = false;
    this->step = 0;
    this->rivalIndex = -1;
}

bool GameState::hasActiveGame() {
    return this->isGameBegun;
}

bool GameState::hasMoves() {
    bool result;
    this->gameBoard->findAvailableMoves();
    result = !this->gameBoard->noMovesNow();
    this->gameBoard->flushLists();
    return result;
}

void GameState::resetPathmap() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            this->pathMap[i][j] = PathMapMarker::Nothing;
        }
    }
}

void GameState::makeGame(GameParameters parameters) {
    if (this->hasActiveGame()) this->endGame();
    this->gameBoard = this->makeBoard(parameters.playerSide);
    this->rival =
        (parameters.gameType == GameType::RivalIsACPU) ?
            this->makeRival(
                parameters.difficulty,
                Auxiliary::invertColor(parameters.playerSide)
            ) :
            nullptr;
    this->rivalIndex = Auxiliary::convertDifficultyToIndex(parameters.difficulty);
    this->playerSide = parameters.playerSide;
    this->activeMetaArray = this->gameBoard->makeMetaArray();
    this->isGameBegun = true;
}

CLCEngine::Board* GameState::makeBoard(CLCEngine::CheckerColor playerSide) {
    return new CLCEngine::Board(playerSide);
}

CLCEngine::AbstractComputerRival* GameState::makeRival(
    CLCEngine::Difficulty difficulty,
    CLCEngine::CheckerColor side
) {
    if (difficulty == CLCEngine::Difficulty::Dumbass) {
        return new CLCEngine::ComputerRivalRandom(side);
    } else {
        return new CLCEngine::ComputerRivalMinimax(difficulty, side);
    }
}

void GameState::endGame() {
    for (int i = 0; i < 8; i++) {
        delete [] this->activeMetaArray[i];
    }
    delete [] this->activeMetaArray;

    delete this->gameBoard;

    delete this->rival;

    this->setAsNull();
}

void GameState::findMovesIfRequired() {
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

void GameState::fillPathMap(CLCEngine::Coordinates source) {
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

void GameState::appendMove(CLCEngine::Move move) {
    this->currentSequence.append(move);
    this->applyMoveOnMetaArray(move);
    this->step++;
}

void GameState::applyMoveOnMetaArray(CLCEngine::Move move) {
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

void GameState::filterNearestMovesWith(CLCEngine::Move move) {
    this->nearestMoves = this->filter(move);
}

CLCEngine::MoveList GameState::filter(CLCEngine::Move move) {
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

bool GameState::noNearestMoves() {
    return this->nearestMoves.getLength() == 0;
}

void GameState::commitMove() {
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
    //this->currentLogLine.clear();

    this->activeMetaArray = this->gameBoard->makeMetaArray();
}

bool GameState::hasMoveBeenMade() {
    return this->moveHasBeenMade;
}

CLCEngine::MoveList GameState::makeRegularCPUMove() {
    CLCEngine::MoveList theBestSequence = this->rival->getNextSequence(*(this->gameBoard));

    if (theBestSequence.getLength() == 0) {
        CLCEngine::MoveSequencesList nearestSequences = this->gameBoard->getAvailableSequences();
        std::uniform_int_distribution<> emergency(0, nearestSequences.getLength() - 1);
        theBestSequence = nearestSequences.getElement(emergency(eng));

    }

    return theBestSequence;
}
void GameState::commitCPUMove(CLCEngine::MoveList sequence) {
    this->gameBoard->makeASequence(sequence);
    this->gameBoard->flushLists();
    this->gameBoard->passTurn(); //send delete > controller

    for (int i = 0; i < 8; i++) {
        delete [] this->activeMetaArray[i];
    }
    delete [] this->activeMetaArray;

    this->activeMetaArray = this->gameBoard->makeMetaArray();

    this->moveHasBeenMade = false;
}

CLCEngine::CheckerColor GameState::whoIsPlayer() {
    return this->gameBoard->whoIsPlayer();
}
CLCEngine::CheckerColor GameState::currentMover() {
    return this->gameBoard->whoMoves();
}

void GameState::flushMetaArray() {
    for (int i = 0; i < 8; i++) {
        delete [] this->activeMetaArray[i];
    }
    delete [] this->activeMetaArray;

    this->activeMetaArray = this->gameBoard->makeMetaArray();
    this->moveHasBeenMade = false;
}

void GameState::setDrawOfferingState(bool state) {
    this->drawHasBeenOffered = state;
}

bool GameState::requestCPURivalDrawAgreement() {
    return glist(eng) < AppConst::cpuDrawChances[this->rivalIndex];
}

bool GameState::isDrawAvailable() {
    return !this->drawHasBeenOffered;
}

int GameState::getRivalIndex() {
    return this->rivalIndex;
}
