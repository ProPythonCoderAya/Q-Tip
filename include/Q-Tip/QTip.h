//
// Created by Ayaan on 2026-08-23.
//

#ifndef QTIP_QTIP_H
#define QTIP_QTIP_H

#include <Q-Tip/Graphics/Renderer.h>
#include <Q-Tip/Graphics/Color.h>
#include <Q-Tip/Graphics/Shapes/Shapes.h>
#include <Q-Tip/Graphics/Font.h>
#include <Q-Tip/Graphics/Texture.h>
#include <Q-Tip/Clock.h>

#include <Q-Tip/Window/Window.h>

union SDL_Event;

QTIP_CODE_BEGIN

class QTipRuntime {
public:
    QTipRuntime();

    ~QTipRuntime();

    void pollEvents() const;

private:
    SDL_Event* _event;

    std::vector<Window*> _windows;

    void registerWindow(Window* window);
    void unregisterWindow(Window* window);

    void replaceWindow(Window* oldWindow, Window* newWindow);

    friend class Window;
};

inline QTipRuntime QTRuntime;

QTIP_CODE_END

#endif //QTIP_QTIP_H
