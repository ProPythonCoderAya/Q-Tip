//
// Created by Ayaan on 2026-08-30.
//

#ifndef QTIP_TEXTBOX_H
#define QTIP_TEXTBOX_H
#include <string>
#include <Q-Tip/Config.h>

#include "Q-Tip/Graphics/Font.h"
#include "Q-Tip/Graphics/Shapes/Rect.h"
#include "Q-Tip/Window/Window.h"

QTIP_CODE_BEGIN

class Textbox {
public:
    Textbox(float x, float y, float width, float height, const std::optional<Font>& font);
    Textbox(Rect rect, const std::optional<Font>& font);

    std::string text;

    void render(Window window);

    void handleEvent(const SDL_Event& event);

    void setActive(bool active);
    [[nodiscard]] bool getActive() const;
private:
    Rect _rect{};
    Font _font;
    float _fontHeight;
    bool _active = false;
    void setMinHeight();
    void deleteCharacter();
};

QTIP_CODE_END

#endif //QTIP_TEXTBOX_H
