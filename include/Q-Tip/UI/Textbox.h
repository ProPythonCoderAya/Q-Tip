//
// Created by Ayaan on 2026-08-30.
//

#ifndef QTIP_TEXTBOX_H
#define QTIP_TEXTBOX_H
#include <string>
#include <Config.h>

#include "Q-Tip/Graphics/Font.h"
#include "Q-Tip/Graphics/Shapes/Rect.h"
#include "Q-Tip/Window/Window.h"

QTIP_CODE_BEGIN

class Textbox : public UIObject {
public:
    Textbox(float x, float y, float width, float height, const std::optional<Font>& font);
    Textbox(Rect rect, const std::optional<Font>& font);

    void render(Window& window);

    void handleEvent(const SDL_Event& event) override;

    void setActive(bool active);
    [[nodiscard]] bool getActive() const;

    void type(std::string text);

    [[nodiscard]] std::string getText() const;
    void setText(const std::string& text);
private:
    Rect _rect{};
    Font _font;
    float _fontHeight = 0;
    size_t _caretPosition = 0;
    bool _active = false;
    uint64_t _blinkTimer = 0;
    std::string _text;
    void setMinHeight();
    void deleteCharacter();
    [[nodiscard]] size_t getBytePosition() const;
    [[nodiscard]] size_t getCaretPosition(float mouseX) const;
};

QTIP_CODE_END

#endif //QTIP_TEXTBOX_H
