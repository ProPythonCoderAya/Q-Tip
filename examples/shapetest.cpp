#include <Q-Tip/QTip.h>
#include <SDL3/SDL.h>

#include <cmath>
#include <string>
#include <random>
#include <vector>
#include <algorithm>

using namespace QTip;

namespace {
    std::random_device rd;
    std::mt19937 gen(rd());

    Point randomPoint(float width, float height) {
        std::uniform_real_distribution<float> distribX(0.0f, width);
        std::uniform_real_distribution<float> distribY(0.0f, height);
        return {distribX(gen), distribY(gen)};
    }

    Color randomColor() {
        std::uniform_int_distribution<uint8_t> distrib(0, 255);
        return {distrib(gen), distrib(gen), distrib(gen), 255};
    }

    struct MovingTriangle {
        Triangle triangle;
        Color color;

        float vx;
        float vy;
    };

    // Move every point in the triangle.
    void moveTriangle(MovingTriangle& tri, float dt) {
        for (auto& point : tri.triangle) {
            point.x += tri.vx * dt;
            point.y += tri.vy * dt;
        }
    }

    // Bounce the triangle off the window edges.
    void bounceTriangle(MovingTriangle& tri, float width, float height) {
        float minX = tri.triangle[0].x;
        float maxX = tri.triangle[0].x;
        float minY = tri.triangle[0].y;
        float maxY = tri.triangle[0].y;

        for (const auto& point : tri.triangle) {
            minX = std::min(minX, point.x);
            maxX = std::max(maxX, point.x);
            minY = std::min(minY, point.y);
            maxY = std::max(maxY, point.y);
        }

        // Hit left/right edge.
        if (minX < 0.0f && tri.vx < 0.0f) {
            tri.vx = -tri.vx;

            // Move it back inside the window.
            float amount = -minX;
            for (auto& point : tri.triangle)
                point.x += amount;
        }
        else if (maxX > width && tri.vx > 0.0f) {
            tri.vx = -tri.vx;

            float amount = maxX - width;
            for (auto& point : tri.triangle)
                point.x -= amount;
        }

        // Hit top/bottom edge.
        if (minY < 0.0f && tri.vy < 0.0f) {
            tri.vy = -tri.vy;

            float amount = -minY;
            for (auto& point : tri.triangle)
                point.y += amount;
        }
        else if (maxY > height && tri.vy > 0.0f) {
            tri.vy = -tri.vy;

            float amount = maxY - height;
            for (auto& point : tri.triangle)
                point.y -= amount;
        }
    }
}

int main() {
    Window window("Q-Tip Shape Test", 800, 600);

    Font font("assets/JetBrainsMono-Regular.ttf", 16);
    Font largeFont("assets/JetBrainsMono-Regular.ttf", 28);

    Texture texture(window.getRenderer(), "assets/weirdstockphoto5-1.jpg");

    Uint64 previousTime = SDL_GetTicks();

    std::vector<MovingTriangle> triangles;

    std::uniform_real_distribution<float> speed(-200.0f, 200.0f);

    // Use the initial window size when creating the triangles.
    for (int i = 0; i < 100; i++) {
        triangles.push_back({
            {
                randomPoint(window.width(), window.height()),
                randomPoint(window.width(), window.height()),
                randomPoint(window.width(), window.height())
            },
            randomColor(),
            speed(gen),
            speed(gen)
        });
    }

    while (!window.shouldClose()) {
        window.pollEvents();

        // Delta time in seconds.
        Uint64 currentTime = SDL_GetTicks();
        float dt = static_cast<float>(currentTime - previousTime) / 1000.0f;
        previousTime = currentTime;

        // Avoid huge movement if the program freezes for a moment.
        dt = std::min(dt, 0.05f);

        // IMPORTANT: get the current window dimensions every frame.
        float width = window.width();
        float height = window.height();

        window->setRenderColor(Color::black);
        window->clear();

        for (auto& triangle : triangles) {
            window->setRenderColor(triangle.color);
            window->renderTriangle(triangle.triangle);
            moveTriangle(triangle, dt);
            bounceTriangle(triangle, width, height);
        }

        window->present();
    }

    return 0;
}