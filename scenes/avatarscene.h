#ifndef AVATARSCENE_H
#define AVATARSCENE_H

#include <clcengine2/checker/CheckerMetaInfo.h>
#include <QGraphicsScene>
#include <QPixmap>

class AvatarScene : public QGraphicsScene {
        Q_OBJECT
    public:
        explicit AvatarScene(QString assetsPath, QObject *parent = nullptr);
        void drawAvatar(int index);
        void setScaling(CLCEngine::Coordinates scaling);

    private:
        QString avatarsNames[9] = {
            "abugan.jpg",
            "nerd.jpg",
            "wojak.jpg",
            "tai_lung.png",
            "jotaro.png",
            "eggman.png",
            "user.jpg",
            "chad.jpg",
            "none.jpg"
        };
        QPixmap avatars[9];
        void loadAvatars(QString assetsPath);
        CLCEngine::Coordinates scaling;
};
#endif // AVATARSCENE_H
