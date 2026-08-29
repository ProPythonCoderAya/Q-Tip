//
// Created by Ayaan on 2026-08-23.
//

#include "Q-Tip/QTip.h"

#include <iostream>
#include <SDL3_ttf/SDL_ttf.h>

#include "include/Helpers.h"

QTipRuntime::QTipRuntime() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        QTipLog(fmt("SDL_Init failed: %s", SDL_GetError()), LOG_FATAL);
        exit(1);
    }

    if (!TTF_Init()) {
        QTipLog(fmt("TTF_Init failed: %s", SDL_GetError()), LOG_FATAL);
        exit(1);
    }
}

QTipRuntime::~QTipRuntime() {
    TTF_Quit();
    SDL_Quit();
}
