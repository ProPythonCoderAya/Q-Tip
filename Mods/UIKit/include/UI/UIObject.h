//
// Created by Ayaan on 2026-08-31.
//

#ifndef QTIP_UIOBJECT_H
#define QTIP_UIOBJECT_H

#include <Q-Tip/Config.h>
#include <Q-Tip/Window/Window.h>

union SDL_Event;
struct SDL_Window;

QTIP_CODE_BEGIN

class UIObject {
MODDABLE_ROOT(UIObject)

public:
    virtual ~UIObject() = default;

    virtual void handleEvent(const SDL_Event& event) = 0;
protected:
    static SDL_Window* window(Window& window) {
        return window;
    }
};

QTIP_CODE_END

#endif //QTIP_UIOBJECT_H
