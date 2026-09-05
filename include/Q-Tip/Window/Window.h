//
// Created by Ayaan on 2026-08-23.
//

#ifndef QTIP_WINDOW_H
#define QTIP_WINDOW_H

#include <optional>
#include <Q-Tip/Config.h>
#include <Q-Tip/Graphics/Renderer.h>

#include "Q-Tip/Window/Input.h"
#include "Q-Tip/UI/UIObject.h"

struct SDL_Window;
union SDL_Event;

QTIP_CODE_BEGIN

class Window {
public:
    Window(const char* title, float width, float height);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(Window&& other) noexcept;
    Window& operator=(Window&& other) noexcept;

    void destroy();

    Renderer* operator->();

    void show();
    void hide();

    void setTitle(const char* title);
    void setSize(float width, float height);

    [[nodiscard]] float width() const;
    [[nodiscard]] float height() const;
    [[nodiscard]] Point size() const;

    [[nodiscard]] bool shouldClose() const;

    void addUIObject(UIObject* uiObject);

    Renderer& getRenderer();

    Input& input();

    void pollEvents();

private:
    SDL_Window* _window = nullptr;
    SDL_Event* _event = nullptr;
    std::optional<Renderer> _renderer;
    std::vector<UIObject*> _uiObjects;
    Input _input{};
    float _width = 0;
    float _height = 0;
    bool _shouldClose = false;

    friend class Texture;
};

QTIP_CODE_END

#endif //QTIP_WINDOW_H
