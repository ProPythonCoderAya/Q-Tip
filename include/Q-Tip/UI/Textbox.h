//
// Created by Ayaan on 2026-08-30.
//

#ifndef QTIP_TEXTBOX_H
#define QTIP_TEXTBOX_H

#include <algorithm>
#include <optional>
#include <string>
#include <vector>

#include <Config.h>

#include "PrivateTypes/Lazy.h"
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

    void resize(float width, float height);
    void resize(const Point& size);
    [[nodiscard]] Point getSize() const;

private:
    Rect _rect{};
    Font _font;

    float _fontHeight = 0;

    // Character/codepoint position, NOT byte position.
    size_t _caretPosition = 0;

    // Used by up/down arrows so the caret tries to preserve
    // its horizontal position when moving between lines.
    size_t _preferredColumn = 0;

    bool _active = false;
    uint64_t _blinkTimer = 0;

    std::string _text;

    Point _scroll = {0, 0};

    void setMinHeight();

    void deleteCharacter();

    [[nodiscard]]
    size_t getBytePosition() const;

    [[nodiscard]]
    size_t getCaretPosition(float mouseX, float mouseY) const;

    [[nodiscard]]
    size_t getLineStartByte(size_t line) const;

    [[nodiscard]]
    size_t getLineEndByte(size_t line) const;

    [[nodiscard]]
    size_t getLineStartCaret(size_t line) const;

    [[nodiscard]]
    size_t getLineEndCaret(size_t line) const;

    [[nodiscard]]
    size_t getCurrentLine() const;

    [[nodiscard]]
    size_t getCurrentColumn() const;

    [[nodiscard]]
    size_t getCaretPositionFromLineColumn(
        size_t line,
        size_t column
    ) const;

    [[nodiscard]]
    size_t getLineCount() const;

    [[nodiscard]]
    size_t getLineColumnFromCaret(
        size_t caretPosition
    ) const;

    [[nodiscard]]
    size_t getNextCharacterBytePosition(size_t bytePosition) const;

    [[nodiscard]]
    size_t getPreviousCharacterBytePosition(size_t bytePosition) const;

    [[nodiscard]]
    std::string getLine(size_t line) const;

    [[nodiscard]]
    std::string getLineBeforeCaret() const;

    [[nodiscard]]
    float getCaretX() const;

    [[nodiscard]]
    float getCaretY() const;

    [[nodiscard]]
    bool needsScrollX() const;

    [[nodiscard]]
    bool needsScrollY() const;

    void scrollCaretIntoView();

    Lazy<Point> _textSize{[this] {
        float width = 0;
        float height = _fontHeight;

        std::string line;

        for (char c : _text) {
            if (c == '\n') {
                width = std::max(
                    width,
                    _font.getTextWidth(line)
                );

                line.clear();
                height += _fontHeight;
            } else {
                line += c;
            }
        }

        width = std::max(
            width,
            _font.getTextWidth(line)
        );

        return Point{width, height};
    }, &_text};

    Lazy<Point> _scrollMaxSize{[this] {
        const Point size = _textSize;

        const float x =
            std::max(
                0.0f,
                size.x - (_rect.size.x - 10.0f)
            );

        const float y =
            std::max(
                0.0f,
                size.y - (_rect.size.y - 10.0f)
            );

        return Point{x, y};
    }, &_rect, &_text};
};

QTIP_CODE_END

#endif //QTIP_TEXTBOX_H