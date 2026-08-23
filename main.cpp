#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>

#include "Helpers.h"

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    if (!TTF_Init()) {
        std::cerr << "TTF_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    constexpr int WIDTH = 800;
    constexpr int HEIGHT = 600;
    SDL_Window* window = SDL_CreateWindow("CLion Project Manager",
                              WIDTH, HEIGHT,
                              SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("assets/JetBrainsMono-Regular.ttf", 16);
    if (!font) {
        std::cerr << "TTF_OpenFont failed: " << SDL_GetError() << "\n";
        return 1;
    }

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        renderTextCentered(
            font,
            renderer,
            "Hello, world!",
            SDL_Color{255,255,255,255},
            WIDTH / 2,
            HEIGHT / 2
        );

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();

    return 0;
}
