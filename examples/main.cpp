#include <Q-Tip/QTip.h>
#include <Q-Tip/Graphics/Font.h>
#include <SDL3/SDL.h>

using namespace QTip;

int main() {
    Window window("Q-Tip Feature Test", 800, 600);

    Font font("assets/JetBrainsMono-Regular.ttf", 16);
    Font largeFont("assets/JetBrainsMono-Regular.ttf", 28);

    Texture texture(window.getRenderer(), "assets/calender.svg");

    const Uint64 startTime = SDL_GetTicks();

    while (!window.shouldClose()) {
        window.pollEvents();

        const float width = window.width();
        const float height = window.height();

        const Uint64 elapsed = SDL_GetTicks() - startTime;
        const float t = static_cast<float>(elapsed) / 1000.0f;

        window->setRenderColor(Color::black);
        window->clear();

        window->setRenderColor(Color::blue);
        window->renderRect({20.0f, 20.0f, width - 40.0f, height - 40.0f}, false);

        window->setRenderColor(Color::red);
        window->renderRect({50.0f, 80.0f, 180.0f, 100.0f});

        window->setRenderColor(Color::green);
        window->renderRect({260.0f, 80.0f, 180.0f, 100.0f}, false);

        window->setRenderColor(Color::white);
        window->renderLine(50.0f, 230.0f, width - 50.0f, 230.0f);
        window->renderLine(50.0f, 250.0f, width - 50.0f, height - 80.0f);

        window->renderTexture(texture, Rect::zero, {200.0f, 200.0f, 200.0f, 200.0f});

        window->setRenderColor(Color::red);
        for (int i = 0; i < 40; ++i) {
            const float x = 60.0f + static_cast<float>(i) * 8.0f;
            const float y = 300.0f + SDL_sinf(t * 3.0f + static_cast<float>(i) * 0.35f) * 35.0f;
            window->renderPoint(x, y);
        }

        window->renderText(
            font,
            "renderText(): top-left aligned text",
            60.0f,
            height - 160.0f,
            Color::white
        );

        window->renderText(
            font,
            "Window features: show(), hide(), setTitle(), setSize(), width(), height(), pollEvents(), shouldClose()",
            60.0f,
            height - 130.0f,
            Color::green
        );

        window->renderText(
            font,
            "Renderer features: clear(), present(), setRenderColor(), renderPoint(), renderLine(), renderRect()",
            60.0f,
            height - 100.0f,
            Color::blue
        );

        window->renderTextCentered(
            largeFont,
            "Q-Tip Feature Test",
            width / 2.0f,
            45.0f,
            Color::white
        );

        window->renderTextCentered(
            font,
            "Close the window to exit",
            width / 2.0f,
            height - 45.0f,
            Color::red
        );

        window->present();
    }

    return 0;
}
