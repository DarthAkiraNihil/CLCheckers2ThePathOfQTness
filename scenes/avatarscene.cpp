#include "avatarscene.h"
#include "boardscene.h"

AvatarScene::AvatarScene(AssetLoader* assetLoader, QObject *parent) : QGraphicsScene{parent} {
    this->assetLoader = assetLoader;
    this->scaling = INVALID_PLACE;
}

void AvatarScene::drawAvatar(int index) {
    if (!(this->scaling == INVALID_PLACE)) {
        this->addPixmap(
            this->assetLoader->getAvatar(index).scaled(
                this->scaling.x,
                this->scaling.y
            )
        );
    }
}

void AvatarScene::setScaling(CLCEngine::Coordinates scaling) {
    this->scaling = scaling;
}
