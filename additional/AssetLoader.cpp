#include <constants.h>
#include "AssetLoader.h"

AssetLoader::AssetLoader(QString assetsPath)
{
    for (int i = 0; i < 9; i++) {
        this->assets[i] = QPixmap(
            assetsPath + "\\" + AppConst::standardAssetsNames[i]
        );
    }
    for (int i = 0; i < 9; i++) {
        this->avatars[i] = QPixmap(
            assetsPath + "\\cpu\\" + AppConst::avatarsNames[i]
        );
    }
}

QPixmap AssetLoader::getChecker(CLCEngine::CheckerMetaInfo metaInfo) {
    return this->assets[this->getAssetIndex(metaInfo)];
}

int AssetLoader::getAssetIndex(CLCEngine::CheckerMetaInfo metaInfo) {
    int colorDefiner = (metaInfo.color == CLCEngine::CheckerColor::Black) ? 0 : 2,
        typeDefiner = (metaInfo.type == CLCEngine::CheckerType::Common) ? 0: 1;
    return 1 + colorDefiner + typeDefiner;
}

QPixmap AssetLoader::getLogo() {
    return this->assets[8];
}

QPixmap AssetLoader::getBoard() {
    return this->assets[0];
}

QPixmap AssetLoader::getCursor() {
    return this->assets[5];
}

QPixmap AssetLoader::getPathmapMarker(PathMapMarker marker) {
    return this->assets[(marker == PathMapMarker::Destination) ? 6 : 7];
}

QPixmap AssetLoader::getAvatar(int index) {
    return this->avatars[index];
}

