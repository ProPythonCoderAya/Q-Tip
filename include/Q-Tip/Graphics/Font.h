//
// Created by Ayaan on 2026-08-23.
//

#ifndef QTIP_FONT_H
#define QTIP_FONT_H

#include <SDL3_ttf/SDL_ttf.h>
#include <Q-Tip/Config.h>

QTIP_CODE_BEGIN

class Font {
public:
    explicit Font(const char* path, float size);
    ~Font();

    Font(const Font&) = delete;
    Font& operator=(const Font&) = delete;

    Font(Font&& other) noexcept;
    Font& operator=(Font&& other) noexcept;

    void destroy();

    operator TTF_Font*() const;
private:
    TTF_Font* _font = nullptr;
};

QTIP_CODE_END

#endif //QTIP_FONT_H
