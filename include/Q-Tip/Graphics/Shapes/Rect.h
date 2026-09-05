//
// Created by Ayaan on 2026-08-23.
//

#ifndef QTIP_RECT_H
#define QTIP_RECT_H
#include "../../Config.h"
#include "Q-Tip/Math/Point.h"

struct SDL_FRect;

QTIP_CODE_BEGIN

struct Rect {
    Point origin;
    Point size;

    operator SDL_FRect() const;

    bool operator==(const Rect& other) const {
        return origin.x == other.origin.x && origin.y == other.origin.y && size.x == other.size.x && size.y == other.size.y;
    }

    [[nodiscard]] bool isPointInside(const Point& point) const {
        return origin.x <= point.x && point.x <= origin.x + size.x && origin.y <= point.y && point.y <= origin.y + size.y;
    }

    static const Rect zero;
};

inline const Rect Rect::zero = {0, 0, 0, 0};

QTIP_CODE_END

#endif //QTIP_RECT_H
