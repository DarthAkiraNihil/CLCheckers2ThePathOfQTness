#ifndef MAKENEWGAMEDIALOG_H
#define MAKENEWGAMEDIALOG_H

#include <auxTypes/gameparameters.h>
#include <scenes/avatarscene.h>
#include <QDialog>

namespace Ui {
    class MakeNewGameDialog;
}

class MakeNewGameDialog : public QDialog {
    Q_OBJECT

    public:
        explicit MakeNewGameDialog(QString avatarAssetsPath, QWidget *parent = nullptr);
        ~MakeNewGameDialog();
        GameParameters makeANewGame();
        QDialog::DialogCode getStatus();
    private:
        Ui::MakeNewGameDialog *ui;
        GameParameters currentParams;
        QDialog::DialogCode status;

        CLCEngine::Difficulty auxGetDifficultyByIndex(int index);
        CLCEngine::CheckerColor auxGeneratePlayerSide();

        QPixmap assets[8];
        QString
        standardAssetsNames[8] = {
            "board.png",
            "black.png",
            "black_king.png",
            "white.png",
            "white_king.png",
            "chosen_frame.png",
            "destination_frame.png",
            "victim_frame.png"
        },
        cpuNames[6] = {
            "Неандерталец Абуган Бунганов Уганович",
            "Нерде Моджи",
            "Вояк",
            "Тайлунг",
            "Джотаро Куджо",
            "Доктор Айво \"Эггман\" Роботник",
        },
        cpuDescs[6] = {
            "\"Уга-буга? Буга-уга-уга-бука! Абуга!\"\n\n"
            "Самый неумелый игрок в шашки, что есть на белом свете. Что ж, ничего не могу сказать. Должно быть просто. Удачи :)\n\n"
            "Но всё же не недооценивайте его, порой его ходы довольно опасны.",
            "\"Эм, сэр, вы должны учесть, что я ещё не умею играть в шашки хорошо. Вы должны дать мне фору!\"\n\n"
            "Не самый умелый игрок, однако он уже дальше смотрит на игровую ситуацию, потому его ходы немного опаснее, чем у предыдущего.\n\n"
            "Однако, уверен, его просто одолеть. Но будьте готовы к его ультразанудству. Удачи :)",
            "\"Я был взращен энергией мемов, возносящих мой интеллект, потому я уверен, что мне будет несложно победить\"\n\n"
            "Хм, а вот это уже из категории умелых. Довольно зазнавшийся, однако его слова полностью подтверждаются его умением играть. Его ходы могут сыграть свою роль очень нескоро, однако его комбинаторное зрение развито значительно лучше.\n\n"
            "Его непросто будет одолеть. И всё же, удачи! :)",
            "\"Наконец-то, достойный соперник. Наша схватка будет легендарной!\"\n\n"
            "Он просидел в темнице более 10 лет, оттачивая мастерство игры в шашки. Потому он способен просчитывать всё далеко вперёд, дальше, чем предыдущий противник.\n\n"
            "Будьте начеку, когда делаете ходы, ведь они могут с лёгкостью подставить вас в критическое положение.\n\n"
            "Будьте осторожны! Удачи!",
            "\"Ставлю душу своей матери на то, что я одолею тебя\"\n\n"
            "Крайне опасный противник, который настолько уверен в себе, что вводит своих оппонентов в ступор. Поистине великолепно играет в шашки, и умеет смотреть далеко вперёд, просчитывая миллионы ходов за несколько секунд.\n\n"
            "Не поддайтесь его невозмутимости, так как он этим воспользуется и одолеет вас в считанные минуты.\n\n"
            "Удачи!",
            "\"ХА-ХА-ХА-ХА! ТЫ ОПОЗДАЛ СОНИК! ТЕПЕРЬ Я СЕРТИФИЦИРОВАННАЯ ПРОГРАММА ДЛЯ ИГРЫ В РУССКИЕ ШАШКИ! БУАХА-ХА-ХА-ХА!\"\n\n"
            "Самый опасный враг, хотя, говорят, есть и опаснее...\n\n"
            "В любом случае, IQ Доктора равен 300 баллам, потому его ходы настолько гениальны, что вы ни за что их не просчитаете. Его комбинаторное зрение совершенно, оно не знает изъянов. Хотя сам доктор долго думает.\n\n"
            "Он уже давно хотел вызвать кого-нибудь на поединок в шашки, потому ваша задача - одолеть его, ведь иначе он всех зверей превратит в бандиков.\n\n"
            "Покажите ему, кто здесь истинный мастер игры в русские шашки!",
        };
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
