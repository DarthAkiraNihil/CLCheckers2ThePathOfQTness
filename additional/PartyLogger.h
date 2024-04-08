#include <QObject>
#include <QString>
#include <clcengine2/clcengine2.h>

#ifndef PARTYLOGGER_H
#define PARTYLOGGER_H


class PartyLogger: public QObject
{
    Q_OBJECT
    public:
        PartyLogger();

    signals:
        void logLineCommited(QString line);

    private:
        CLCEngine::DynamicSequence<std::string> gameLog{};
        std::string currentLogLine{};


        CLCEngine::Coordinates getNotationSquareCoordinates(CLCEngine::Coordinates place);
    public slots:
        void registerMove(CLCEngine::Move move);
        void registerSequence(CLCEngine::MoveList sequence);
        void commitLogLine();
        void writeLog(std::string filename);
        void importLog(CLCEngine::DynamicSequence<std::string> imported);
};

#endif // PARTYLOGGER_H
