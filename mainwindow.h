#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "makenewgamedialog.h"
#include <scenes/avatarscene.h>
#include <scenes/boardscene.h>
#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>
#include <clcengine2/clcengine2.h>
//#include <constants.h>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QString assetsPath, QWidget *parent = nullptr);
        ~MainWindow();
    private:
        Ui::MainWindow *ui;
        MakeNewGameDialog* dialog;
        BoardScene* boardScene;
        GameState* stateRef;

        AssetLoader loader;

        void keyPressEvent(QKeyEvent *event);
        void beginGame();
        AvatarScene *avatar1, *avatar2;
        QString playerName, rivalName;
    signals:
        void invokeEndgame();

    private slots:

        void on_ngButton_clicked();
        void on_ruleButton_clicked();
        void on_controlsButton_clicked();
        void on_loadButton_clicked();
        void on_saveButton_clicked();
        void on_offerDrawButton_clicked();
        void on_surrenderButton_clicked();

        //void causeUpdate();

        void setRivalAvatar(int index);
        void setPlayerName(QString name);
        void setRivalName(QString name);

        void showEndgamePlayerWon();
        void showEndgameCPURivalWon(QString message);
        void showEndgameHumanRivalWon();

        void changeStatusBarText(QString text);
        void logMove(QString moveLine);
};
#endif // MAINWINDOW_H
