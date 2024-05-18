//
// Created by EgrZver on 03.10.2023.
//

#ifndef CLCENGINE2_CHECKERMETAINFO_H
#define CLCENGINE2_CHECKERMETAINFO_H

namespace CLCEngine {
    struct Coordinates {
        int x, y;

        bool operator==(Coordinates const& other) const {
            return this->x == other.x && this->y == other.y;
        }
    };

    enum class CheckerColor {
        White,
        Black,
        NoColor
    };

    enum class CheckerType {
        Common,
        King,
        NoType
    };



    struct CheckerMetaInfo {
        Coordinates coordinates;
        CheckerColor color;
        CheckerType type;
        bool markedForDeath;

        bool operator==(CheckerMetaInfo const& other) const {
            return (this->coordinates == other.coordinates) &&
                (this->color == other.color) &&
                (this->type == other.type) &&
                (this->markedForDeath == other.markedForDeath);

        }
    };

    typedef CheckerMetaInfo** MetaArray;

    const CheckerMetaInfo NO_VICTIM = {{-1, -1},
                                       CheckerColor::NoColor,
                                       CheckerType::NoType,
                                       false};

    enum class Direction {
            Up = 1,
            Right = 1,
            Down = -1,
            Left = -1
    };
}

#endif //CLCENGINE2_CHECKERMETAINFO_H
