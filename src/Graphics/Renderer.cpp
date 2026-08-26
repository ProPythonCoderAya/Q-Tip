//
// Created by Ayaan on 2026-08-23.
//

#include <Q-Tip/Graphics/Renderer.h>
#include <Q-Tip/Graphics/Texture.h>
#include "Helpers.h"

QTIP_CODE_BEGIN

Renderer::Renderer(SDL_Window* window) {
    if (!window) return;
    _renderer = SDL_CreateRenderer(window, nullptr);
    if (!_renderer) {
        QTipLog(fmt("SDL_CreateRenderer failed: %s", SDL_GetError()), LOG_FATAL);
        exit(1);
    }
}

Renderer::~Renderer() {
    destroy();
}

Renderer::Renderer(Renderer&& other) noexcept
    : _renderer(other._renderer) {
    other._renderer = nullptr;
}

Renderer& Renderer::operator=(Renderer&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    destroy();

    _renderer = other._renderer;
    other._renderer = nullptr;

    return *this;
}

void Renderer::destroy() {
    if (!_renderer) return;
    SDL_DestroyRenderer(_renderer);
    _renderer = nullptr;
}

Renderer::operator SDL_Renderer*() const {
    return _renderer;
}

SDL_Renderer* Renderer::nativeHandle() const {
    return _renderer;
}

void Renderer::clear() {
    SDL_RenderClear(_renderer);
}

void Renderer::present() {
    SDL_RenderPresent(_renderer);
}

void Renderer::setRenderColor(Color color) {
    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
}

void Renderer::renderPoint(float x, float y) {
    SDL_RenderPoint(_renderer, x, y);
}

void Renderer::renderLine(float x1, float y1, float x2, float y2) {
    SDL_RenderLine(_renderer, x1, y1, x2, y2);
}

void Renderer::renderRect(Rect rect, bool filled) {
    SDL_FRect sdlRect = rect;

    if (filled)
        SDL_RenderFillRect(_renderer, &sdlRect);
    else
        SDL_RenderRect(_renderer, &sdlRect);
}

void Renderer::renderText(const Font& font, const char* text, float x, float y, Color color) {
    ::renderText(font, _renderer, text, color, x, y);
}

void Renderer::renderTextCentered(const Font& font, const char* text, float x, float y, Color color) {
    ::renderTextCentered(font, _renderer, text, color, x, y);
}

void Renderer::renderTexture(Texture& texture, Rect srcRect, Rect dstRect) {
    SDL_FRect src;
    SDL_FRect dst;
    SDL_FRect* srcPtr = nullptr;
    SDL_FRect* dstPtr = nullptr;
    if (srcRect != Rect::zero) {
        src = srcRect;
        srcPtr = &src;
    }
    if (dstRect != Rect::zero) {
        dst = dstRect;
        dstPtr = &dst;
    }
    SDL_RenderTexture(
        _renderer,
        texture.nativeHandle(),
        srcPtr,
        dstPtr
    );
}

QTIP_CODE_END
