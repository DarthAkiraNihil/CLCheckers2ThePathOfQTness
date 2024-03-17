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

        void writeLog(std::string filename);
        CLCEngine::Coordinates getNotationSquareCoordinates(CLCEngine::Coordinates place);
    public slots:
        void registerMove(CLCEngine::Move move);
        void registerSequence(CLCEngine::MoveList sequence);
        void commitLogLine();
};

#endif // PARTYLOGGER_H
