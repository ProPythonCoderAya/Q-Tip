#include <SDL3/SDL.h>
#include <Q-Tip/QTip.h>

#include "Q-Tip/Graphics/Font.h"

int main() {
    constexpr int WIDTH = 800;
    constexpr int HEIGHT = 600;
    QTip::Window window("Q-Tip", WIDTH, HEIGHT);

    QTip::Font font("assets/JetBrainsMono-Regular.ttf", 16);

    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        window->setRenderColor(QTip::Color::black);
        window->clear();

        window->renderTextCentered(
            font,
            "Hello, world!",
            WIDTH / 2.0,
            HEIGHT / 2.0,
            QTip::Color::white
        );

        window->present();
    }

    return 0;
}
