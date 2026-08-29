//
// Created by Ayaan on 2026-08-23.
//

#include <Q-Tip/Graphics/Font.h>
#include <include/Helpers.h>
#include <SDL3_ttf/SDL_ttf.h>

QTIP_CODE_BEGIN

Font::Font(const char* path, float size) {
    _font = TTF_OpenFont(path, size);
    if (!_font) {
        QTipLog(fmt("TTF_OpenFont failed: %s", SDL_GetError()), LOG_ERROR);
    }
}

Font::~Font() {
    destroy();
}

Font::Font(Font&& other) noexcept
    : _font(other._font) {
    other._font = nullptr;
}

Font& Font::operator=(Font&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    destroy();

    _font = other._font;
    other._font = nullptr;

    return *this;
}

void Font::destroy() {
    if (!_font) return;
    TTF_CloseFont(_font);
    _font = nullptr;
}

Font::operator TTF_Font*() const {
    return _font;
}

QTIP_CODE_END
