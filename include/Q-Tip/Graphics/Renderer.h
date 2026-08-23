//
// Created by Ayaan on 2026-08-23.
//

#ifndef QTIP_RENDERER_H
#define QTIP_RENDERER_H

#include <SDL3/SDL.h>
#include <Q-Tip/Config.h>
#include <Q-Tip/Graphics/Color.h>
#include <Q-Tip/Graphics/Rect.h>

#include "Font.h"

QTIP_CODE_BEGIN

class Renderer {
public:
    explicit Renderer(SDL_Window* window);
    ~Renderer();

    void destroy();

    void clear();
    void present();

    void setRenderColor(Color color);

    void renderPoint(float x, float y);
    void renderLine(float x1, float y1, float x2, float y2);
    void renderRect(Rect rect, bool filled = true);

    void renderText(const Font& font, const char* text, float x, float y, Color color);
    void renderTextCentered(const Font& font, const char* text, float x, float y, Color color);

private:
    SDL_Renderer* _renderer = nullptr;
};

QTIP_CODE_END

#endif //QTIP_RENDERER_H
