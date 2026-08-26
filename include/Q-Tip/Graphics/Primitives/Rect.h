//
// Created by Ayaan on 2026-08-23.
//

#ifndef QTIP_RECT_H
#define QTIP_RECT_H
#include "Q-Tip/Config.h"

QTIP_CODE_BEGIN

struct Rect {
    float x, y, w, h;

    operator SDL_FRect() const {
        return {x, y, w, h};
    }

    bool operator==(const Rect& other) const {
        return x == other.x && y == other.y && w == other.w && h == other.h;
    }

    static const Rect zero;
};

inline const Rect Rect::zero = {0, 0, 0, 0};

QTIP_CODE_END

#endif //QTIP_RECT_H
