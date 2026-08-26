//
// Created by Ayaan on 2026-08-26.
//

#ifndef QTIP_TRIANGLE_H
#define QTIP_TRIANGLE_H
#include "Q-Tip/Config.h"
#include "Q-Tip/Math/Point.h"

QTIP_CODE_BEGIN

struct Triangle {
    Point a, b, c;

    bool operator==(const Triangle& other) const {
        return a == other.a && b == other.b && c == other.c;
    }
};

QTIP_CODE_END

#endif //QTIP_TRIANGLE_H
