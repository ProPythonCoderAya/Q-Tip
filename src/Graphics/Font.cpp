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

    _meta.path = path;
    _meta.size = size;
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
    _meta = other._meta;
}

Font& Font::operator=(Font&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    destroy();

    _font = other._font;
    other._font = nullptr;
    _meta = other._meta;

    return *this;
}

void Font::destroy() {
    if (!_font) return;
    TTF_CloseFont(_font);
    _font = nullptr;
}

float Font::getFontHeight() const {
    return static_cast<float>(TTF_GetFontHeight(_font));
}

Point Font::getTextSize(const std::string& text) const {
    int width, height;
    TTF_GetStringSize(_font, text.c_str(), 0, &width, &height);
    return {static_cast<float>(width), static_cast<float>(height)};
}

float Font::getTextWidth(const std::string& text) const {
    return getTextSize(text).x;
}

float Font::getTextHeight(const std::string& text) const {
    return getTextSize(text).y;
}

Font::operator TTF_Font*() const {
    return _font;
}

QTIP_CODE_END
