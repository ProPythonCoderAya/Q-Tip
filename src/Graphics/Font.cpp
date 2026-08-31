//
// Created by Ayaan on 2026-08-23.
//

#include <Q-Tip/Graphics/Font.h>
#include <include/Helpers.h>
#include <SDL3_ttf/SDL_ttf.h>

QTIP_CODE_BEGIN

Font::Font(fs::path path, float size) {
    _font = TTF_OpenFont(path.c_str(), size);
    if (!_font) {
        QTipLog(fmt("TTF_OpenFont failed: %s", SDL_GetError()), LOG_ERROR);
    }
}

Font::~Font() {
    destroy();
}

Font::Font(const Font& other) {
    copy(other);
}

Font& Font::operator=(const Font& other) {
    if (this == &other)
        return *this;

    copy(other);

    return *this;
}

void Font::copy(const Font& other) {
    TTF_Font* newFont =
        TTF_OpenFont(other._meta.path.c_str(), other._meta.size);

    if (!newFont) {
        QTipLog(fmt("TTF_OpenFont failed: %s", SDL_GetError()), LOG_ERROR);
        return;
    }

    destroy();

    _meta = other._meta;
    _font = newFont;
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

int Font::getFontHeight() const {
    return TTF_GetFontHeight(_font);
}

Font::operator TTF_Font*() const {
    return _font;
}

QTIP_CODE_END
