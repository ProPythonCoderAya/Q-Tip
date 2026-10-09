//
// Created by Ayaan on 2026-08-23.
//

#include "Q-Tip/QTip.h"

#include <iostream>
#include <SDL3_ttf/SDL_ttf.h>

#include "include/Helpers.h"
#include <Q-Tip/Logger/Log.h>
#include "Q-Tip/Mods/ModLoader/ModLoader.h"

QTIP_CODE_BEGIN

QTipRuntime QTipRuntime::instance;

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

void QTipRuntime::pollEvents() {
    for (Window* window : instance._windows)
        window->beginFrame();

    while (SDL_PollEvent(instance._event)) {
        const SDL_Event& event = *instance._event;

        ModLoader::handleEvent(event);

        if (event.type == SDL_EVENT_QUIT) {
            for (Window* window : instance._windows)
                window->handleEvent(event);

            continue;
        }

        SDL_WindowID windowID = SDL_GetWindowID(SDL_GetWindowFromEvent(&event));

        for (Window* window : instance._windows) {
            if (window->_id == windowID) {
                window->handleEvent(event);
                break;
            }
        }
    }
}

std::vector<Window*> QTipRuntime::windows() {
    return instance._windows;
}

void QTipRuntime::registerWindow(Window* window) {
    instance._windows.push_back(window);
}

void QTipRuntime::unregisterWindow(Window* window) {
    std::erase(instance._windows, window);
}

void QTipRuntime::replaceWindow(Window* oldWindow, Window* newWindow) {
    const auto it = std::ranges::find(instance._windows, oldWindow);

    if (it != instance._windows.end()) {
        *it = newWindow;
        return;
    }

    instance._windows.push_back(newWindow);
}

QTIP_CODE_END
