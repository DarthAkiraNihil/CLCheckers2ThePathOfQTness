#include "PartyLogger.h"
#include <fstream>
#include <QDebug>

PartyLogger::PartyLogger()
{

}

void PartyLogger::registerMove(CLCEngine::Move move) {
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

void PartyLogger::registerSequence(CLCEngine::MoveList sequence) {
    for (int i = 0; i < sequence.getLength(); i++) {
        this->registerMove(sequence.getElement(i));
    }
}

void PartyLogger::commitLogLine() {
    this->gameLog.append(this->currentLogLine);
    qDebug() << "JEB: Made move: " << this->currentLogLine.c_str();
    if (!this->currentLogLine.empty())
    emit this->logLineCommited(this->currentLogLine.c_str());
    this->currentLogLine.clear();
}

void PartyLogger::writeLog(std::string filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        qDebug() << "Error while opening the file";
    } else {
        for (int i = 0; i < this->gameLog.getLength(); i++) {
            out << this->gameLog.getElement(i) << "\n";
        }
        out.close();
        this->gameLog.clear();
    }
    //out.open(filename, std::ios::out);


}

void PartyLogger::importLog(CLCEngine::DynamicSequence<std::string> imported) {
    for (int i = 0; i < imported.getLength(); i++) {
        this->gameLog.append(imported.getElement(i));
    }
}
