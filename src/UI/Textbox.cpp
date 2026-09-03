//
// Created by Ayaan on 2026-08-30.
//

#include "Q-Tip/UI/Textbox.h"

#include <numbers>
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

void Textbox::render(Window& window) {
    Renderer& renderer = window.getRenderer();

    SDL_Texture* target = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        _rect.size.x,
        _rect.size.y
    );

    SDL_SetRenderTarget(renderer, target);

    renderer.setRenderColor({40, 40, 40});
    renderer.renderRoundedRect({0, 0, _rect.size}, 10);

    float rx = 0;
    float ry = 0;
    float rh = _rect.size.y;
    renderer.renderText(_font, _text.c_str(), rx + 5, ry + (rh - _fontHeight) / 2, Color::white);
    const size_t bytePosition = getBytePosition();
    float w = _font.getTextWidth(_text.substr(0, bytePosition));

    if (_active) {
        uint64_t ticks = SDL_GetTicks();
        if ((ticks - _blinkTimer) / 500 % 2 == 0) {
            Rect rect{};
            rect.origin.x = rx + w + 5;
            rect.origin.y = ry + (rh - _fontHeight) / 2.0f;
            rect.size.x = 2;
            rect.size.y = _fontHeight;
            renderer.setRenderColor({255, 255, 255});
            renderer.renderRect(rect);
        }
    }

    SDL_SetRenderTarget(renderer, nullptr);

    SDL_FRect dstRect = _rect;

    SDL_RenderTexture(renderer, target, nullptr, &dstRect);

    SDL_DestroyTexture(target);
}

void Textbox::handleEvent(const SDL_Event& event) {
    switch (event.type) {
    case SDL_EVENT_MOUSE_BUTTON_DOWN: {
        if (event.button.button != SDL_BUTTON_LEFT)
            break;

        if (_rect.isPointInside({event.button.x, event.button.y})) {
            _active = true;
            _caretPosition = getCaretPosition(event.button.x);
            _blinkTimer = SDL_GetTicks();
        } else {
            _active = false;
        }

        break;
    }
    case SDL_EVENT_KEY_DOWN: {
        if (!_active) break;

        switch (event.key.key) {
        default:
            break;
        case SDLK_BACKSPACE:
            deleteCharacter();
            break;
        case SDLK_LEFT:
            if (_caretPosition > 0)
                --_caretPosition;
            break;
        case SDLK_RIGHT:
            if (getBytePosition() < _text.size())
                ++_caretPosition;
            break;
        }

        _blinkTimer = SDL_GetTicks();

        break;
    }
    case SDL_EVENT_TEXT_INPUT: {
        if (!_active)
            break;

        type(event.text.text);

        _blinkTimer = SDL_GetTicks();

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

void Textbox::type(std::string text) {
    const size_t bytePosition = getBytePosition();

    _text.insert(bytePosition, text);

    // Count UTF-8 codepoints added.
    size_t addedCharacters = 0;

    for (size_t i = 0; text[i] != '\0';) {
        auto c = static_cast<unsigned char>(text[i]);

        if ((c & 0b10000000) == 0)
            i += 1;
        else if ((c & 0b11100000) == 0b11000000)
            i += 2;
        else if ((c & 0b11110000) == 0b11100000)
            i += 3;
        else
            i += 4;

        ++addedCharacters;
    }

    _caretPosition += addedCharacters;
}

std::string Textbox::getText() const {
    return _text;
}

void Textbox::setText(const std::string& text) {
    _text = text;
}

void Textbox::setMinHeight() {
    _rect.size.y = std::max(_fontHeight, _rect.size.y);
}

void Textbox::deleteCharacter() {
    const size_t bytePosition = getBytePosition();

    if (bytePosition == 0)
        return;

    size_t start = bytePosition - 1;

    while (start > 0 &&
           (static_cast<unsigned char>(_text[start]) & 0b11000000) == 0b10000000)
        --start;

    _text.erase(start, bytePosition - start);
    --_caretPosition;
}

size_t Textbox::getBytePosition() const {
    size_t bytePosition = 0;

    for (size_t i = 0; i < _caretPosition && bytePosition < _text.size(); ++i) {
        auto c = static_cast<unsigned char>(_text[bytePosition]);

        if ((c & 0b10000000) == 0)
            bytePosition += 1;
        else if ((c & 0b11100000) == 0b11000000)
            bytePosition += 2;
        else if ((c & 0b11110000) == 0b11100000)
            bytePosition += 3;
        else if ((c & 0b11111000) == 0b11110000)
            bytePosition += 4;
    }

    return bytePosition;
}

size_t Textbox::getCaretPosition(float mouseX) const {
    const float textX = _rect.origin.x + 5.0f;

    if (mouseX <= textX)
        return 0;

    size_t bytePosition = 0;
    size_t caretPosition = 0;

    while (bytePosition < _text.size()) {
        const size_t nextBytePosition = [&] {
            const unsigned char c =
                static_cast<unsigned char>(_text[bytePosition]);

            if ((c & 0b10000000) == 0)
                return bytePosition + 1;
            if ((c & 0b11100000) == 0b11000000)
                return bytePosition + 2;
            if ((c & 0b11110000) == 0b11100000)
                return bytePosition + 3;

            return bytePosition + 4;
        }();

        const float currentX =
            textX + _font.getTextWidth(_text.substr(0, bytePosition));

        const float nextX =
            textX + _font.getTextWidth(_text.substr(0, nextBytePosition));

        const float midpoint = (currentX + nextX) / 2.0f;

        if (mouseX < midpoint)
            return caretPosition;

        bytePosition = nextBytePosition;
        ++caretPosition;
    }

    return caretPosition;
}

QTIP_CODE_END
