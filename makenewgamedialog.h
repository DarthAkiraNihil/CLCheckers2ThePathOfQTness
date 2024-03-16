#ifndef MAKENEWGAMEDIALOG_H
#define MAKENEWGAMEDIALOG_H

#include <auxTypes/gameparameters.h>
#include <scenes/avatarscene.h>
#include <QDialog>
#include <additional/AssetLoader.h>

namespace Ui {
    class MakeNewGameDialog;
}

class MakeNewGameDialog : public QDialog {
    Q_OBJECT

    public:
        explicit MakeNewGameDialog(AssetLoader* assetLoader, QWidget *parent = nullptr);
        ~MakeNewGameDialog();
        GameParameters makeANewGame();
        QDialog::DialogCode getStatus();
    private:
        Ui::MakeNewGameDialog *ui;
        GameParameters currentParams;
        QDialog::DialogCode status;
        AvatarScene avatarScene;
    signals:
        void transferAvatarIndex(int index);
        void transferPlayerName(QString name);
        void transferRivalName(QString name);
    private slots:
        void on_rbVSHuman_clicked();
        void on_rbVSCPU_clicked();
        void on_buttonBox_accepted();
        void on_buttonBox_rejected();
        void on_comboBox_currentIndexChanged(int index);
        void on_sideButtonRandom_clicked();
        void on_sideButtonWhite_clicked();
        void on_sideButtonBlack_clicked();
};

#endif // MAKENEWGAMEDIALOG_H
