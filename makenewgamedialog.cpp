#include "makenewgamedialog.h"
#include "./ui_makenewgamedialog.h"
#include <QDebug>
#include <QInputDialog>
#include <additional/Auxiliary.h>
#include <constants.h>

MakeNewGameDialog::MakeNewGameDialog(AssetLoader* assetLoader, QWidget *parent):
    QDialog(parent),
    ui(new Ui::MakeNewGameDialog),
    avatarScene(assetLoader) {

    ui->setupUi(this);

    this->currentParams = DEFAULT_PARAMS;

    this->currentParams.playerSide = Auxiliary::generatePlayerSide();

    this->status = QDialog::Rejected;
    this->avatarScene.setScaling({128, 128});
    this->ui->rivalPic->setScene(&this->avatarScene);
    this->ui->rivalPic->setSceneRect(0, 0, 128, 128);
    this->avatarScene.drawAvatar(8);
    this->ui->cpuRivalDesc->setText("Вы же играете против человека. Зачем вам знать КОМПЬЮТЕРНОГО противника, если его не существует?");
}

MakeNewGameDialog::~MakeNewGameDialog() {
    delete ui;
}

void MakeNewGameDialog::on_rbVSHuman_clicked() {
    this->currentParams.gameType = GameType::RivalIsAHuman;
    this->avatarScene.drawAvatar(8);
    this->ui->cpuRivalDesc->setText("Вы же играете против человека. Зачем вам знать КОМПЬЮТЕРНОГО противника, если его не существует?");
    this->ui->rivalName->setText("Никто не выбран");
}

void MakeNewGameDialog::on_rbVSCPU_clicked() {
    this->currentParams.gameType = GameType::RivalIsACPU;
    this->currentParams.difficulty = Auxiliary::getDifficultyByIndex(this->ui->comboBox->currentIndex());
    this->avatarScene.drawAvatar(this->ui->comboBox->currentIndex());
    this->ui->rivalName->setText(AppConst::cpuNames[this->ui->comboBox->currentIndex()]);
    this->ui->cpuRivalDesc->setText(AppConst::cpuDescs[this->ui->comboBox->currentIndex()]);
}

void MakeNewGameDialog::on_buttonBox_accepted() {
    this->status = QDialog::Accepted;
}

void MakeNewGameDialog::on_buttonBox_rejected() {
    this->status = QDialog::Rejected;
}

void MakeNewGameDialog::on_comboBox_currentIndexChanged(int index) {
    if (this->currentParams.gameType == GameType::RivalIsACPU) {
        this->currentParams.difficulty = Auxiliary::getDifficultyByIndex(index);
        this->avatarScene.drawAvatar(index);
        this->ui->rivalName->setText(AppConst::cpuNames[index]);

        this->ui->cpuRivalDesc->setText(AppConst::cpuDescs[index]);
    }

}

void MakeNewGameDialog::on_sideButtonRandom_clicked() {
    this->currentParams.playerSide = Auxiliary::generatePlayerSide();
}


void MakeNewGameDialog::on_sideButtonWhite_clicked() {
    this->currentParams.playerSide = CLCEngine::CheckerColor::White;
}


void MakeNewGameDialog::on_sideButtonBlack_clicked() {
    this->currentParams.playerSide = CLCEngine::CheckerColor::Black;
}

QDialog::DialogCode MakeNewGameDialog::getStatus() {
    return this->status;
}

GameParameters MakeNewGameDialog::makeANewGame() {
    this->exec();
    GameParameters toReturn = this->currentParams;
    this->currentParams = DEFAULT_PARAMS;
    this->ui->rbVSHuman->setChecked(true);
    this->ui->sideButtonRandom->setChecked(true);

    if (this->status == Accepted) {
        if (toReturn.gameType == GameType::RivalIsACPU) {
            emit this->transferAvatarIndex(this->ui->comboBox->currentIndex());
            emit this->transferRivalName(AppConst::cpuNames[this->ui->comboBox->currentIndex()]);
        } else {
            bool ok;
            QString secondRival = QInputDialog::getText(
                this,
                "Стоять! А как хоть зовут-то второго игрока?",
                "Введите имя второго игрока, если хотите",
                QLineEdit::Normal,
                "Name",
                &ok
            );
            if (!ok || secondRival.isEmpty()) {
                emit this->transferRivalName("(безымянный)");
            } else {
                emit this->transferRivalName(secondRival);
            }
        }

        QString name = this->ui->nameInput->text();
        if (name.trimmed() == "") {
            emit this->transferPlayerName("(безымянный)");
        } else {
            emit this->transferPlayerName(this->ui->nameInput->text());
        }

    }
    this->ui->comboBox->setCurrentIndex(0);
    return toReturn;
}
