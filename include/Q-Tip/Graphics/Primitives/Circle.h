//
// Created by Ayaan on 2026-08-23.
//

#ifndef QTIP_CIRCLE_H
#define QTIP_CIRCLE_H
#include "Q-Tip/Config.h"
#include "Q-Tip/Math/Point.h"

QTIP_CODE_BEGIN
    struct Circle {
    Point center;
    float r;

    bool operator==(const Circle& other) const {
        return center.x == other.center.x && center.y == other.center.y && r == other.r;
    }
};

QTIP_CODE_END

#endif //QTIP_CIRCLE_H
