//
// Created by Ayaan on 2026-08-23.
//

#include "../../include/Q-Tip/Window/Window.h"

#include <utility>

#include "Helpers.h"

QTIP_CODE_BEGIN

Window::Window(const char* title, float width, float height) {
    _window = SDL_CreateWindow(title, static_cast<int>(width), static_cast<int>(height), SDL_WINDOW_RESIZABLE); // resizable default for now
    if (!_window) {
        QTipLog(fmt("SDL_CreateWindow failed: %s", SDL_GetError()), LOG_FATAL);
        exit(1);
    }
    _width = width;
    _height = height;

    _renderer.emplace(_window);
}

Window::~Window() {
    destroy();
}

Window::Window(Window&& other) noexcept
    : _window(other._window),
      _event(other._event),
      _renderer(std::move(other._renderer)),
      _width(other._width),
      _height(other._height),
      _shouldClose(other._shouldClose) {
    other._window = nullptr;
    other._width = 0;
    other._height = 0;
    other._shouldClose = true;
}

Window& Window::operator=(Window&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    destroy();

    _window = other._window;
    _event = other._event;
    _renderer = std::move(other._renderer);
    _width = other._width;
    _height = other._height;
    _shouldClose = other._shouldClose;

    other._window = nullptr;
    other._width = 0;
    other._height = 0;
    other._shouldClose = true;

    return *this;
}

void Window::destroy() {
    if (_renderer) {
        _renderer->destroy();
        _renderer.reset();
    }

    if (!_window) {
        return;
    }

    SDL_DestroyWindow(_window);
    _window = nullptr;
}

Renderer* Window::operator->() {
    return &_renderer.value();
}

void Window::show() {
    SDL_ShowWindow(_window);
}

void Window::hide() {
    SDL_HideWindow(_window);
}

void Window::setTitle(const char* title) {
    SDL_SetWindowTitle(_window, title);
}

void Window::setSize(float width, float height) {
    SDL_SetWindowSize(_window, static_cast<int>(width), static_cast<int>(height));
    _width = width;
    _height = height;
}

[[nodiscard]] float Window::width() const {
    return _width;
}

[[nodiscard]] float Window::height() const {
    return _height;
}

[[nodiscard]] bool Window::shouldClose() const {
    return _shouldClose;
}

void Window::pollEvents() {
    while (SDL_PollEvent(&_event)) {
        switch (_event.type) {
        case SDL_EVENT_QUIT: {
            _shouldClose = true;
            break;
        }
        case SDL_EVENT_WINDOW_RESIZED: {
            _width = static_cast<float>(_event.window.data1);
            _height = static_cast<float>(_event.window.data2);
            break;
        }
        default:
            break;
        }
    }
}

QTIP_CODE_END
