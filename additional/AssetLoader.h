#include <QString>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <clcengine2/checker/CheckerMetaInfo.h>
#include <auxTypes/pathmapmarker.h>

#ifndef ASSETLOADER_H
#define ASSETLOADER_H


class AssetLoader
{
    public:
        AssetLoader(QString assetsPath);
        QPixmap getChecker(CLCEngine::CheckerMetaInfo metaInfo);
        QPixmap getLogo();
        QPixmap getBoard();
        QPixmap getCursor();
        QPixmap getPathmapMarker(PathMapMarker marker);
        QPixmap getAvatar(int index);
    private:
        int getAssetIndex(CLCEngine::CheckerMetaInfo metaInfo);
        QPixmap assets[9], avatars[9];
};

#endif // ASSETLOADER_H
