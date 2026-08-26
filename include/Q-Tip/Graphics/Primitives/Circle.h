//
// Created by Ayaan on 2026-08-23.
//

#ifndef QTIP_RECT_H
#define QTIP_RECT_H
#include "Q-Tip/Config.h"

QTIP_CODE_BEGIN

struct Circle {
    float x, y, r;

    bool operator==(const Circle& other) const {
        return x == other.x && y == other.y && r == other.r;
    }
};

QTIP_CODE_END

#endif //QTIP_RECT_H
