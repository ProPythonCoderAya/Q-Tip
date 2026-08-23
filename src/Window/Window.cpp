//
// Created by Ayaan on 2026-08-23.
//

#include "../../include/Q-Tip/Window/Window.h"

#include <iostream>

#include "Helpers.h"

QTIP_CODE_BEGIN

Window::Window(const char* title, int width, int height) {
    _window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE); // resizable default for now
    if (!_window) {
        QTipLog(fmt("SDL_CreateWindow failed: %s", SDL_GetError()), LOG_FATAL);
        exit(1);
    }

    _renderer.emplace(_window);
}

Window::~Window() {
    destroy();
}

void Window::destroy() {
    _renderer->destroy();

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

void Window::setSize(int width, int height) {
    SDL_SetWindowSize(_window, width, height);
}

[[nodiscard]] int Window::width() const {
    return _width;
}

[[nodiscard]] int Window::height() const {
    return _height;
}

[[nodiscard]] bool Window::shouldClose() const {
    return _shouldClose;
}

void Window::pollEvents() {
    while (SDL_PollEvent(&_event)) {
        if (_event.type == SDL_EVENT_QUIT) {
            _shouldClose = true;
        }
    }
}

QTIP_CODE_END
