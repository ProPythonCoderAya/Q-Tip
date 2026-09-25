//
// Created by Ayaan on 2026-08-23.
//

#include "Q-Tip/QTip.h"

#include <iostream>
#include <SDL3_ttf/SDL_ttf.h>

#include "include/Helpers.h"
#include "Q-Tip/Mods/ModLoader/ModLoader.h"

QTIP_CODE_BEGIN
    QTipRuntime::QTipRuntime() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        QTipLog(fmt("SDL_Init failed: %s", SDL_GetError()), LOG_FATAL);
        exit(1);
    }

    if (!TTF_Init()) {
        QTipLog(fmt("TTF_Init failed: %s", SDL_GetError()), LOG_FATAL);
        exit(1);
    }

    _event = new SDL_Event;
}

QTipRuntime::~QTipRuntime() {
    TTF_Quit();
    SDL_Quit();

    delete _event;
    _event = nullptr;
}

void QTipRuntime::pollEvents() const {
    for (Window* window : _windows)
        window->beginFrame();

    while (SDL_PollEvent(_event)) {
        const SDL_Event& event = *_event;

        ModLoader::handleEvent(event);

        if (event.type == SDL_EVENT_QUIT) {
            for (Window* window : _windows)
                window->handleEvent(event);

            continue;
        }

        SDL_WindowID windowID = 0;

        switch (event.type) {
        case SDL_EVENT_WINDOW_SHOWN:
        case SDL_EVENT_WINDOW_HIDDEN:
        case SDL_EVENT_WINDOW_EXPOSED:
        case SDL_EVENT_WINDOW_MOVED:
        case SDL_EVENT_WINDOW_RESIZED:
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
        case SDL_EVENT_WINDOW_MINIMIZED:
        case SDL_EVENT_WINDOW_MAXIMIZED:
        case SDL_EVENT_WINDOW_RESTORED:
        case SDL_EVENT_WINDOW_MOUSE_ENTER:
        case SDL_EVENT_WINDOW_MOUSE_LEAVE:
        case SDL_EVENT_WINDOW_FOCUS_GAINED:
        case SDL_EVENT_WINDOW_FOCUS_LOST:
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
            windowID = event.window.windowID;
            break;

        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
            windowID = event.key.windowID;
            break;

        case SDL_EVENT_TEXT_INPUT:
            windowID = event.text.windowID;
            break;

        case SDL_EVENT_MOUSE_MOTION:
            windowID = event.motion.windowID;
            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP:
            windowID = event.button.windowID;
            break;

        case SDL_EVENT_MOUSE_WHEEL:
            windowID = event.wheel.windowID;
            break;

        default:
            break;
        }

        if (windowID == 0)
            continue;

        for (Window* window : _windows) {
            if (window->_id == windowID) {
                window->handleEvent(event);
                break;
            }
        }
    }
}

void QTipRuntime::registerWindow(Window* window) {
    _windows.push_back(window);
}

void QTipRuntime::unregisterWindow(Window* window) {
    std::erase(_windows, window);
}

void QTipRuntime::replaceWindow(Window* oldWindow, Window* newWindow) {
    const auto it = std::ranges::find(_windows, oldWindow);

    if (it != _windows.end()) {
        *it = newWindow;
        return;
    }

    _windows.push_back(newWindow);
}

QTIP_CODE_END
