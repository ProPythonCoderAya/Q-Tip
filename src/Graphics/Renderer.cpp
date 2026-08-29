//
// Created by Ayaan on 2026-08-23.
//

#include <Q-Tip/Graphics/Renderer.h>
#include <Q-Tip/Graphics/Texture.h>
#include "Helpers.h"
#include "earcut.hpp"

namespace mapbox {
namespace util {

    // Registrera Index 0 (X)
    template <>
    struct nth<0, QTip::Point> {
        inline static double get(const QTip::Point& p) { return static_cast<double>(p.x); };
    };

    // Registrera Index 1 (Y)
    template <>
    struct nth<1, QTip::Point> {
        inline static double get(const QTip::Point& p) { return static_cast<double>(p.y); };
    };

}
}

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

void Renderer::renderPoint(Point p) {
    SDL_RenderPoint(_renderer, p.x, p.y);
}

void Renderer::renderLine(Line line) {
    SDL_RenderLine(_renderer, line.start.x, line.start.y, line.end.x, line.end.y);
}

void Renderer::renderRect(Rect rect, bool filled) {
    SDL_FRect sdlRect = rect;

    if (filled)
        SDL_RenderFillRect(_renderer, &sdlRect);
    else
        SDL_RenderRect(_renderer, &sdlRect);
}

void Renderer::renderCircle(Circle circle, bool filled) {
    DrawCircle(_renderer, circle.center.x, circle.center.y, circle.radius);
}

void Renderer::renderTriangle(Triangle triangle, bool filled) {
    SDL_Vertex verts[3];
    float r, g, b, a;
    SDL_GetRenderDrawColorFloat(_renderer, &r, &g, &b, &a);
    verts[0] = {{triangle.a.x, triangle.a.y}, {r, g, b, a}, {0, 0}};
    verts[1] = {{triangle.b.x, triangle.b.y}, {r, g, b, a}, {0, 0}};
    verts[2] = {{triangle.c.x, triangle.c.y}, {r, g, b, a}, {0, 0}};
    SDL_RenderGeometry(_renderer, NULL, verts, 3, NULL, 0);
}

void Renderer::renderPolygon(Polygon polygon, bool filled)
{
    if (polygon.points.size() < 3)
        return;

    if (!filled) {
        for (size_t i = 0; i < polygon.points.size(); ++i) {
            const Point& a = polygon.points[i];
            const Point& b = polygon.points[(i + 1) % polygon.points.size()];

            renderLine({a, b});
        }

        return;
    }

    std::vector<std::vector<Point>> rings = {
        polygon.points
    };

    const auto indices = mapbox::earcut<uint32_t>(rings);

    for (size_t i = 0; i < indices.size(); i += 3) {
        const Point& a = polygon.points[indices[i]];
        const Point& b = polygon.points[indices[i + 1]];
        const Point& c = polygon.points[indices[i + 2]];

        renderTriangle({a, b, c});
    }
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
