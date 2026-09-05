//
// Created by Ayaan on 2026-08-23.
//

#ifndef QTIP_RENDERER_H
#define QTIP_RENDERER_H

#include <Q-Tip/Config.h>
#include <Q-Tip/Graphics/Color.h>

#include "Font.h"
#include "Shapes/Circle.h"
#include "Shapes/Triangle.h"
#include "Shapes/Rect.h"
#include "Shapes/Polygon.h"
#include "Shapes/Line.h"

struct SDL_Window;
struct SDL_Renderer;

QTIP_CODE_BEGIN

class Texture;

class Renderer {
public:
    explicit Renderer(SDL_Window* window);
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    Renderer(Renderer&& other) noexcept;
    Renderer& operator=(Renderer&& other) noexcept;

    void destroy();

    operator SDL_Renderer*() const;

    void clear();
    void present();

    void setRenderColor(Color color);

    void renderPoint(Point point);
    void renderLine(Line line);
    void renderRect(Rect rect, bool filled = true);
    void renderRoundedRect(Rect rect, int radius);
    void renderCircle(Circle circle, bool filled = true);
    void renderTriangle(Triangle triangle, bool filled = true);
    void renderPolygon(Polygon polygon, bool filled = true);

    void renderText(const Font& font, const char* text, float x, float y, Color color);
    void renderTextCentered(const Font& font, const char* text, float x, float y, Color color);

    void renderTexture(Texture& texture, Rect srcRect, Rect dstRect);

private:
    SDL_Renderer* _renderer = nullptr;
};

QTIP_CODE_END

#endif //QTIP_RENDERER_H
