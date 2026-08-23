//
// Created by Ayaan on 2026-08-23.
//

#ifndef QTIP_WINDOW_H
#define QTIP_WINDOW_H

#include <optional>
#include <SDL3/SDL.h>
#include <Q-Tip/Config.h>
#include <Q-Tip/Graphics/Renderer.h>

QTIP_CODE_BEGIN

class Window {
public:
    Window(const char* title, int width, int height);
    ~Window();

    void destroy();

    Renderer* operator->();

    void show();
    void hide();

    void setTitle(const char* title);
    void setSize(int width, int height);

    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;

    [[nodiscard]] bool shouldClose() const;

    void pollEvents();

private:
    SDL_Window* _window = nullptr;
    SDL_Event _event{};
    std::optional<Renderer> _renderer;
    int _width = 0;
    int _height = 0;
    bool _shouldClose = false;
};

QTIP_CODE_END

#endif //QTIP_WINDOW_H
