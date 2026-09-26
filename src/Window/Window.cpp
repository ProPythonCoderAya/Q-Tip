//
// Created by Ayaan on 2026-08-23.
//

#include "Q-Tip/Window/Window.h"

#include <utility>

#include "../include/Helpers.h"

#include <SDL3/SDL.h>

#include "Q-Tip/Mods/ModLoader/ModLoader.h"
#include "Q-Tip/QTip.h"

QTIP_CODE_BEGIN

Window::Window(const char* title, float width, float height)
{
    open(title, width, height);
}

Window::~Window()
{
    QTipRuntime::unregisterWindow(this);
    destroy();
}

Window::Window(Window&& other) noexcept
    : _window(other._window),
      _event(other._event),
      _renderer(std::move(other._renderer)),
      _width(other._width),
      _height(other._height),
      _shouldClose(other._shouldClose),
      _input(std::move(other._input)),
      _id(other._id)
{
    QTipRuntime::replaceWindow(&other, this);

    other._window = nullptr;
    other._event = nullptr;
    other._width = 0;
    other._height = 0;
    other._shouldClose = true;
}

Window& Window::operator=(Window&& other) noexcept
{
    if (this == &other) {
        return *this;
    }

    // Remove this object from the runtime before destroying
    // the resources it currently owns.
    QTipRuntime::unregisterWindow(this);

    destroy();

    _window = other._window;
    _event = other._event;
    _renderer = std::move(other._renderer);
    _width = other._width;
    _height = other._height;
    _shouldClose = other._shouldClose;
    _input = std::move(other._input);
    _id = other._id;

    // Transfer other's runtime registration to this object.
    QTipRuntime::replaceWindow(&other, this);

    other._window = nullptr;
    other._event = nullptr;
    other._width = 0;
    other._height = 0;
    other._shouldClose = true;

    return *this;
}

void Window::destroy()
{
    if (_renderer) {
        _renderer->destroy();
        _renderer.reset();
    }

    delete _event;
    _event = nullptr;

    if (!_window) {
        return;
    }

    SDL_StopTextInput(_window);

    SDL_DestroyWindow(_window);
    _window = nullptr;
}

void Window::open(const char* title, float width, float height) {
    _window = SDL_CreateWindow(
        title,
        static_cast<int>(width),
        static_cast<int>(height),
        SDL_WINDOW_RESIZABLE
    );

    if (!_window) {
        QTipLog(fmt("SDL_CreateWindow failed: %s", SDL_GetError()), LOG_ERROR);
        return;
    }

    _width = width;
    _height = height;

    _renderer.emplace(_window);

    _event = new SDL_Event;

    SDL_StartTextInput(_window);

    _id = SDL_GetWindowID(_window);

    QTipRuntime::registerWindow(this);
}

void Window::close() {
    if (!_window)
        return;

    QTipRuntime::unregisterWindow(this);

    destroy();

    _id = 0;
    _width = 0;
    _height = 0;
    _shouldClose = true;
}

bool Window::isOpen() const {
    return _window != nullptr;
}

Renderer* Window::operator->()
{
    return &_renderer.value();
}

void Window::show()
{
    SDL_ShowWindow(_window);
}

void Window::hide()
{
    SDL_HideWindow(_window);
}

void Window::setTitle(const char* title)
{
    SDL_SetWindowTitle(_window, title);
}

void Window::setSize(float width, float height)
{
    SDL_SetWindowSize(
        _window,
        static_cast<int>(width),
        static_cast<int>(height)
    );

    _width = width;
    _height = height;
}

[[nodiscard]] float Window::width() const
{
    return _width;
}

[[nodiscard]] float Window::height() const
{
    return _height;
}

Point Window::size() const
{
    return {_width, _height};
}

[[nodiscard]] bool Window::shouldClose() const
{
    return _shouldClose;
}

Renderer& Window::getRenderer()
{
    return _renderer.value();
}

Input& Window::input()
{
    return _input;
}

void Window::handleEvent(const SDL_Event& event)
{
    _input.processEvent(event);

    switch (event.type) {
    case SDL_EVENT_QUIT:
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        _shouldClose = true;
        break;

    case SDL_EVENT_WINDOW_RESIZED:
        _width = static_cast<float>(event.window.data1);
        _height = static_cast<float>(event.window.data2);
        break;

    default:
        break;
    }
}

void Window::beginFrame()
{
    _input.beginFrame();
}

Window::operator SDL_Window*() const
{
    return _window;
}

QTIP_CODE_END