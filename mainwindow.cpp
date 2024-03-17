#include "mainwindow.h"
#include <auxTypes/cursormovedirections.h>

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include "constants.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QString assetsPath, QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    state(),
    logger(),
    loader(assetsPath) {

    ui->setupUi(this);

    this->boardScene = new BoardScene(&this->loader, &this->state);
    this->dialog = new MakeNewGameDialog(&loader);

    this->ui->boardView->setScene(this->boardScene);

    this->ui->boardView->setSceneRect(0, 0, 488, 488);
    /*
    QObject::connect(
        &this->boardScene,
        &BoardScene::clickHasBeenSomewhere,
        this,
        &MainWindow::test
    );*/

    //this->stateRef = this->boardScene->getStateReference();

    //signal connection
    {
        QObject::connect(
            this->dialog,
            &MakeNewGameDialog::transferAvatarIndex,
            this, &MainWindow::setRivalAvatar
        );
        QObject::connect(
            this->dialog,
            &MakeNewGameDialog::transferPlayerName,
            this,
            &MainWindow::setPlayerName
        );
        QObject::connect(
            this->dialog,
            &MakeNewGameDialog::transferRivalName,
            this, &MainWindow::setRivalName
        );
        QObject::connect(
            this->boardScene,
            &BoardScene::playerWon,
            this,
            &MainWindow::showEndgamePlayerWon
        );
        QObject::connect(
            this->boardScene,
            &BoardScene::cpuRivalWon,
            this,
            &MainWindow::showEndgameCPURivalWon
        );
        QObject::connect(
            this->boardScene,
            &BoardScene::humanRivalWon,
            this,
            &MainWindow::showEndgameHumanRivalWon
        );
        QObject::connect(
            this->boardScene,
            &BoardScene::transferStatusBarText,
            this,
            &MainWindow::changeStatusBarText
        );
        QObject::connect(
            &this->logger,
            &PartyLogger::logLineCommited,
            this,
            &MainWindow::logMove
        );
        QObject::connect(
            this,
            &MainWindow::invokeEndgame,
            &this->state, &GameState::endGame
        );
        QObject::connect(
            this->boardScene,
            &BoardScene::registerMove,
            &this->logger, &PartyLogger::registerMove
        );
        QObject::connect(
            this->boardScene,
            &BoardScene::registerSequence,
            &this->logger, &PartyLogger::registerSequence
        );
        QObject::connect(
            this->boardScene,
            &BoardScene::commitLogLine,
            &this->logger, &PartyLogger::commitLogLine
        );
    }

    //end signal connection

    //avatars setup

    this->avatar1 = new AvatarScene(&this->loader);
    this->avatar2 = new AvatarScene(&this->loader);

    this->avatar2->setScaling({101, 101});
    this->avatar1->setScaling({101, 101});

    this->ui->playerView->setScene(this->avatar1);
    this->ui->rivalView->setScene(this->avatar2);

    this->ui->playerView->setSceneRect(0, 0, 101, 101);
    this->ui->rivalView->setSceneRect(0, 0, 101, 101);

    // end avatars setup

    this->ui->statusbar->showMessage("Welcome to the club!");

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_ngButton_clicked() {
    if (!this->state.hasActiveGame()) {
        GameParameters parameters = this->dialog->makeANewGame();
        qDebug() << "DIFF: " << (int) parameters.difficulty;
        if (this->dialog->getStatus() == QDialog::Accepted) {
            qDebug() << "JEB: Constructing a game";
            this->state.makeGame(parameters);
            //this->boardScene.setPlayerColor(this->gameBoard->whoIsPlayer());
            this->avatar1->drawAvatar(7);
            if (parameters.gameType == GameType::RivalIsAHuman) {
                this->avatar2->drawAvatar(7);
            }
            this->boardScene->renderContent();
            this->boardScene->doTheFirstCPUMoveIfRequired();
        } else {
            qDebug() << "JEB: Canceled construction";
        }
    } else {
        QMessageBox::critical(nullptr, "Saatana vittu perkele", "Ай-яй! Вы не можете начать новую игру, пока не закончите текущую!");
    }

}

void MainWindow::keyPressEvent(QKeyEvent* event) {

    switch(event->key()) {
        case Qt::Key_W: {
            this->boardScene->moveCursor(CursorMoveDirections::DIRECTION_UP);
            break;
        }
        case Qt::Key_A: {
            this->boardScene->moveCursor(CursorMoveDirections::DIRECTION_LEFT);
            break;
        }
        case Qt::Key_S: {
            this->boardScene->moveCursor(CursorMoveDirections::DIRECTION_DOWN);
            break;
        }
        case Qt::Key_D: {
            this->boardScene->moveCursor(CursorMoveDirections::DIRECTION_RIGHT);
            break;
        }
        case Qt::Key_Return: {
            this->boardScene->handleClick();
            break;
        }
    }
}


void MainWindow::setRivalAvatar(int index) {
    this->avatar2->drawAvatar(index);
}

void MainWindow::setPlayerName(QString name) {
    this->ui->playerName->setText(name);
    this->playerName = name;
}

void MainWindow::setRivalName(QString name) {
    this->ui->rivalName->setText(name);
    this->rivalName = name;
}

void MainWindow::showEndgamePlayerWon() {
    QMessageBox(
        QMessageBox::Icon::Information,
        "Flawless victory!",
        "Вы победили!"
    ).exec();
    emit this->invokeEndgame();
}

void MainWindow::showEndgameCPURivalWon(QString message) {
    QMessageBox(
        QMessageBox::Icon::Information,
        "Flawless victory!",
        message
    ).exec();
    emit this->invokeEndgame();
}

void MainWindow::showEndgameHumanRivalWon() {
    QMessageBox(
        QMessageBox::Icon::Information,
        "Flawless victory!",
        "Oh no"
    ).exec();
    emit this->invokeEndgame();
}

void MainWindow::on_ruleButton_clicked()
{
    QMessageBox(
        QMessageBox::Icon::Information,
        "Правила",
        "Правила:\n"
        "1) Простая шашка ходит по диагонали вперёд на одну клетку.\n"
        "2) Дамка ходит по диагонали на любое свободное поле как вперёд, так и назад.\n"
        "3) При выделении шашки ей необходимо сделать ход, за исключением случаев, когда шашка не имеет ходов.\n"
        "4) Взятие обязательно.\n"
        "5) Простая шашка, находящаяся рядом с шашкой соперника, за которой имеется свободное поле, переносится"
        " через эту шашку на это свободное поле. Если есть возможность продолжить взятие других шашек соперника,"
        " то это взятие продолжается, пока бьющая шашка не достигнет положения, из которого бой невозможен."
        " Взятие простой шашкой производится как вперёд, так и назад.\n"
        "6) Дамка бьёт по диагонали, как вперёд, так и назад, и становится на любое свободное поле после побитой шашки. \n"
        "   Аналогично, дамка может бить несколько фигур соперника и должна бить до тех пор, пока это возможно.\n"
        "7) При бое через дамочное поле простая шашка превращается в дамку и продолжает бой по правилам дамки.\n"
        "8) При нескольких вариантах взятия, например, одну шашку или две, игрок выбирает вариант взятия по своему усмотрению.\n"
        "9) Партия считается выигранной в следующих случаях: \n"
        "    - если у одного из соперников побиты все шашки;\n"
        "    - если один из участников заявил о том, что сдаётся;\n"
        "    - если шашки одного из участников заперты и он не может сделать очередной ход.\n"
        "10) Партия считается закончившейся вничью, если один из участников предлагает ничью, а другой её принимает.\n"
        "11) Белые всегда ходят первыми"
    ).exec();
}


void MainWindow::on_controlsButton_clicked()
{
    QMessageBox(
        QMessageBox::Icon::Information,
        "Управление",
        "Выбор шашки производится с помощью курсора, указанным на доске. Его перемещение осуществляется нажатием \"стрелочек\" на клавиатуре.\n\n"
        "Выбор шашки происходит через нажатие клавиши Enter. Если для выбранной шашки есть доступные поля, к которым можно переместиться, они будут подсвечены зелёным цветом.\n\n"
        "Если одно из таких полей будет выбрано, также через нажатие Enter, будет сделал ход той шашкой, которую вы выбрали до этого.\n\n"
        "Также управление осуществляется с помощью мышки, достаточно просто кликать по клеткам доски"
    ).exec();
}


void MainWindow::logMove(QString moveLine) {
    this->ui->gameLog->addItem(moveLine);
    this->ui->gameLog->scrollToBottom();
}


void MainWindow::on_loadButton_clicked()
{
    if (this->state.hasActiveGame()) {
         QMessageBox::critical(nullptr, "Saatana vittu perkele", "Ай-яй! Вы не можете загрузить игру, ведь в данный момент вы уже играете. Закончите сперва текущую игру!");
    } else {
        QString fname = QFileDialog::getOpenFileName(this, tr("Load Game"),
                                             ".",
                                             tr("Savefiles (*.sav)"));
        if (fname != "") {
            GameSaveData save;
            FILE* inputDataStream = fopen(fname.toLocal8Bit().data(), "rb");
            if (inputDataStream == 0) printf("File of a game save does not exist! Wait... what the hell?");
            else {
                fread(&save, sizeof(GameSaveData), 1, inputDataStream);
                fclose(inputDataStream);
                this->boardScene->importGameData(save);
            }
        }
    }
}


void MainWindow::on_saveButton_clicked()
{
    if (!this->state.hasActiveGame()) {
         QMessageBox::critical(nullptr, "Saatana vittu perkele", "Ай-яй! Вы не можете сохранить игру, ведь у вас нет игры. Начните сперва новую игру!");
    } else {
        QString fname = QFileDialog::getSaveFileName(
            this,
            tr("Save Game"),
            ".",
            tr("Savefiles (*.sav)")
        );
        if (fname != "") {
            GameSaveData save = this->boardScene->exportGameData();
            FILE* saveStream = fopen(fname.toLocal8Bit().data(), "w+b");
            fwrite(&save, sizeof(GameSaveData), 1, saveStream);
            fclose(saveStream);
        }
    }
}


void MainWindow::on_offerDrawButton_clicked()
{
    if (this->state.hasActiveGame()) {
        if (this->state.isDrawAvailable()) {
            if (this->state.isThereACPURival()) {
                if (this->state.requestCPURivalDrawAgreement()) {
                    QMessageBox::information(this, "Успех!", AppConst::drawAcceptMessages[this->state.getRivalIndex()]);
                    emit this->invokeEndgame();
                } else {
                    QMessageBox::critical(this, "Не получилось :(", AppConst::drawDeclineMessages[this->state.getRivalIndex()]);
                }
            } else {
                if (
                    QMessageBox::question(
                        this,
                        "Предложение ничьи",
                        "Вы собираетесь предложить ничью. Ваш оппонент согласен на неё?",
                        QMessageBox::Yes | QMessageBox::No
                    ) == QMessageBox::Yes
                ) {
                    QMessageBox::information(this, "Friendship", "Friendship, friendship, again!");
                    emit this->invokeEndgame();
                } else {
                    QMessageBox::warning(this, "Concurrency", "Concurrency, concurrency, again!");
                }
            }
            this->state.setDrawOfferingState(true);
            //this->boardScene->setDrawAsOffered();
        } else {
            QMessageBox::critical(nullptr, "Saatana vittu perkele", "Ай-яй! Вы не можете предложить ничью, ведь вы уже предлагали её на этом ходе! Попробуйте на следующем ходе.");
        }

    } else {
        QMessageBox::critical(this, "Saatana vittu perkele", "Ай-яй! Вы не можете предложить ничью, ведь у вас нет игры!");
    }
}


void MainWindow::on_surrenderButton_clicked()
{
    if (this->state.hasActiveGame()) {
        if (
            QMessageBox::question(
                this,
                "Одумайтесь!",
                "Вы действительно хотите сдаться?",
                QMessageBox::Yes | QMessageBox::No
            ) == QMessageBox::Yes
        ) {
            QMessageBox::critical(this, "Saatana vittu perkele", "what a dumbass");
            emit this->invokeEndgame();
        }
    } else {
        QMessageBox::critical(this, "Saatana vittu perkele", "Ай-яй! Вы не можете сдаться, ведь у вас нет игры!");
    }
    //this->boardScene.trySurrender();
}

void MainWindow::changeStatusBarText(QString text) {
    this->ui->statusbar->showMessage(text);
}


