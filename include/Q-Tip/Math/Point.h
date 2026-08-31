//
// Created by Ayaan on 2026-08-26.
//

#ifndef QTIP_POINT_H
#define QTIP_POINT_H
#include "Config.h"

QTIP_CODE_BEGIN

struct Point {
    float x, y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

QTIP_CODE_END

#endif //QTIP_POINT_H
