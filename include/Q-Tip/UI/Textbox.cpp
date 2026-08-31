//
// Created by Ayaan on 2026-08-30.
//

#include "Textbox.h"

#include <SDL3_ttf/SDL_ttf.h>

QTIP_CODE_BEGIN
    std::string defaultFontPath() {
#ifdef _WIN32
    return "C:/Windows/Fonts/Arial.ttf";
#elif defined(__linux__)
    return "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
#elif defined(__APPLE__)
    return "/System/Library/Fonts/Supplemental/Arial.ttf";
#else
#error "Unsupported platform"
#endif
}

Textbox::Textbox(float x, float y, float width, float height, const std::optional<Font>& font) : _font(defaultFontPath(), 16) {
    if (font.has_value()) {
        _font.destroy();
        _font = font.value();
    }
    _fontHeight = TTF_GetFontHeight(_font);
    _rect = {x, y, width, height};
}

Textbox::Textbox(const Rect rect, const std::optional<Font>& font) : _font(defaultFontPath(), 16) {
    if (font.has_value()) {
        _font.destroy();
        _font = font.value();
    }
    _fontHeight = TTF_GetFontHeight(_font);
    _rect = rect;
}

void Textbox::render(Window window) {
    Renderer& renderer = window.getRenderer();

    renderer.setRenderColor({40, 40, 40});
    renderer.renderRoundedRect(_rect, 10);
    float rx = _rect.origin.x;
    float ry = _rect.origin.y;
    float rh = _rect.size.y;
    renderer.renderText(_font, text.c_str(), rx + 5, ry + (rh + _fontHeight) / 2, Color::white);
}

QTIP_CODE_END
