#include <Q-Tip/QTip.h>
#include <Q-Tip/UI/Textbox.h>

using namespace QTip;

int main() {
    Window window("Q-Tip Textbox Test", 800, 600);

    Font font("assets/JetBrainsMono-Regular.ttf", 20);

    Textbox textbox(
        100.0f,
        250.0f,
        600.0f,
        50.0f,
        font
    );

    textbox.type("Type something...");

    window.addUIObject(&textbox);

    while (!window.shouldClose()) {
        window.pollEvents();

        window->setRenderColor({20, 20, 20, 255});
        window->clear();

        textbox.render(window);

        window->present();
    }

    return 0;
}
