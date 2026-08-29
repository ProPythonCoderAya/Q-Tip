//
// Created by Ayaan on 2026-08-29.
//

#include <Q-Tip/Graphics/Color.h>
#include <SDL3/SDL.h>

QTIP_CODE_BEGIN

Color::operator ::SDL_Color() const {
    return {r, g, b, a};
}

QTIP_CODE_END
