#include <Q-Tip/QTip.h>
#include <Q-Tip/UI/Textbox.h>
#include <fstream>
#include <sstream>

using namespace QTip;

std::string readFile(const fs::path &path) {
    std::ifstream file(path);
    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}

int main() {
    Window window("Q-Tip Textbox Test", 800, 600);

    Font font("assets/JetBrainsMono-Regular.ttf", 20);

    Textbox textbox(
        10.0f,
        10.0f,
        780.0f,
        580.0f,
        font
    );

    textbox.type("lolo banan");

    Point lastWindowSize{};

    window.addUIObject(&textbox);

    while (!window.shouldClose()) {
        window.pollEvents();

        Point windowSize = window.size();
        if (lastWindowSize != windowSize) {
            lastWindowSize = windowSize;
            textbox.resize(windowSize.x - 20, windowSize.y - 20);
        }

        window->setRenderColor({20, 20, 20, 255});
        window->clear();

        textbox.render(window);

        window->present();
    }

    return 0;
}
