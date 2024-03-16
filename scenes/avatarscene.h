#ifndef AVATARSCENE_H
#define AVATARSCENE_H

#include <clcengine2/checker/CheckerMetaInfo.h>
#include <QGraphicsScene>
#include <QPixmap>
#include <additional/AssetLoader.h>

class AvatarScene : public QGraphicsScene {
        Q_OBJECT
    public:
        explicit AvatarScene(AssetLoader* assetLoader, QObject *parent = nullptr);
        void drawAvatar(int index);
        void setScaling(CLCEngine::Coordinates scaling);

    private:
        AssetLoader* assetLoader;
        CLCEngine::Coordinates scaling;
};
#endif // AVATARSCENE_H
