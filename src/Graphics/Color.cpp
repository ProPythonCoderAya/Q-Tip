//
// Created by Ayaan on 2026-08-29.
//

#include <Q-Tip/Graphics/Color.h>
#include <SDL3/SDL.h>

QTIP_CODE_BEGIN

Color::operator ::SDL_Color() const {
    return {r, g, b, a};
}

Color::operator ::SDL_FColor() const {
    return {static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f, static_cast<float>(a) / 255.0f};
}

QTIP_CODE_END
