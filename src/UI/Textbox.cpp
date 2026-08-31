//
// Created by Ayaan on 2026-08-30.
//

#include "Q-Tip/UI/Textbox.h"

#include <SDL3/SDL.h>

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
    _fontHeight = _font.getFontHeight();
    _rect = {x, y, width, height};
}

Textbox::Textbox(const Rect rect, const std::optional<Font>& font) : _font(defaultFontPath(), 16) {
    if (font.has_value()) {
        _font.destroy();
        _font = font.value();
    }
    _fontHeight = _font.getFontHeight();
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
    float w = _font.getTextWidth(text.substr(_caretPosition));

    if (_active) {
        unsigned int ticks = SDL_GetTicks();
        if (ticks / 500 % 2 == 0) {
            Rect rect{};
            rect.origin.x = rx + w + 5;
            rect.origin.y = ry + (rh - _fontHeight) / 2.0f;
            rect.size.x = 2;
            rect.size.y = _fontHeight;
            renderer.setRenderColor({255, 255, 255});
            renderer.renderRect(rect);
        }
    }
}

void Textbox::handleEvent(const SDL_Event& event) {
    switch (event.type) {
    case SDL_EVENT_MOUSE_BUTTON_DOWN: {
        if (event.button.button == SDL_BUTTON_LEFT) {
            _active = _rect.isPointInside({event.motion.x, event.motion.y});
        }
        break;
    }
    case SDL_EVENT_KEY_DOWN: {
        if (!_active) break;

        if (event.key.key == SDLK_BACKSPACE)
            deleteCharacter();

        break;
    }
    case SDL_EVENT_TEXT_INPUT: {
        if (_active) {
            text += event.text.text;
            _caretPosition++;
        }
        break;
    }
    default: break;
    }
}

void Textbox::setActive(bool active) {
    _active = active;
}

bool Textbox::getActive() const {
    return _active;
}

void Textbox::setMinHeight() {
    _rect.size.y = std::max(_fontHeight, _rect.size.y);
}

void Textbox::deleteCharacter() {
    if (text.empty())
        return;
    size_t i = text.size() - 1;
    while (i > 0 && (static_cast<unsigned char>(text[i]) & 0b11000000) == 0b10000000)
        --i;
    text.erase(i);
    _caretPosition--;
}

QTIP_CODE_END
