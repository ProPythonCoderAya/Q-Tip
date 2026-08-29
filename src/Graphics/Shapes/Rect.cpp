//
// Created by Ayaan on 2026-08-29.
//

#include <Q-Tip/Graphics/Shapes/Rect.h>
#include <SDL3/SDL_rect.h>

QTIP_CODE_BEGIN

Rect::operator SDL_FRect() const {
    return {origin.x, origin.y, size.x, size.y};
}

QTIP_CODE_END
