//
// Created by Ayaan on 2026-08-31.
//

#ifndef QTIP_UIOBJECT_H
#define QTIP_UIOBJECT_H

#include <Config.h>

union SDL_Event;

QTIP_CODE_BEGIN

class UIObject {
public:
    virtual ~UIObject() = default;

    virtual void handleEvent(const SDL_Event& event) = 0;
};

QTIP_CODE_END

#endif //QTIP_UIOBJECT_H
