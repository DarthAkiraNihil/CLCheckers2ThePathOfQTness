#ifndef CURSORMOVEDIRECTIONS_H
#define CURSORMOVEDIRECTIONS_H
#include <clcengine2/checker/CheckerMetaInfo.h>

namespace CursorMoveDirections {
    const CLCEngine::Coordinates DIRECTION_UP = {0, -1};
    const CLCEngine::Coordinates DIRECTION_DOWN = {0, 1};
    const CLCEngine::Coordinates DIRECTION_LEFT = {-1, 0};
    const CLCEngine::Coordinates DIRECTION_RIGHT = {1, 0};
}

#endif // CURSORMOVEDIRECTIONS_H
