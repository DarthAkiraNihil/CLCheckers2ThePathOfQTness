#include "avatarscene.h"
#include "boardscene.h"

AvatarScene::AvatarScene(QString assetsPath, QObject *parent) : QGraphicsScene{parent} {
    this->loadAvatars(assetsPath);
    this->scaling = INVALID_PLACE;
}

void AvatarScene::loadAvatars(QString assetsPath) {
    for (int i = 0; i < 9; i++) {
        this->avatars[i] = QPixmap(
            assetsPath + "\\cpu\\" + this->avatarsNames[i]
        );
    }
}

void AvatarScene::drawAvatar(int index) {
    if (!(this->scaling == INVALID_PLACE)) {
        this->addPixmap(
            this->avatars[index].scaled(
                this->scaling.x,
                this->scaling.y
            )
        );
    }
}

void AvatarScene::setScaling(CLCEngine::Coordinates scaling) {
    this->scaling = scaling;
}
