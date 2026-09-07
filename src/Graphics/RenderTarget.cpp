//
// Created by Ayaan on 2026-09-07.
//

#include "Q-Tip/Graphics/RenderTarget.h"

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>

#include "Q-Tip/Graphics/Renderer.h"

QTIP_CODE_BEGIN

RenderTarget::RenderTarget(Renderer& renderer, int width, int height) : Texture(renderer, "") {
    destroy();

    _texture = SDL_CreateTexture(renderer._renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
}

QTIP_CODE_END
