#include <Q-Tip/Window/Input.h>

QTIP_CODE_BEGIN

namespace {
    constexpr std::size_t mouseButtonIndex(MouseButton button)
    {
        return static_cast<std::size_t>(button);
    }

    constexpr std::size_t keyIndex(Key key)
    {
        return static_cast<std::size_t>(key);
    }
}

Input::Input()
{
    _keys.fill(false);
    _pressed.fill(false);
    _released.fill(false);

    _mouseButtons.fill(false);
    _mousePressed.fill(false);
    _mouseReleased.fill(false);
}

bool Input::keyDown(Key key) const
{
    const auto index = keyIndex(key);

    if (index >= KeyCount)
        return false;

    return _keys[index];
}

bool Input::keyWasPressed(Key key) const
{
    const auto index = keyIndex(key);

    if (index >= KeyCount)
        return false;

    return _pressed[index];
}

bool Input::keyWasReleased(Key key) const
{
    const auto index = keyIndex(key);

    if (index >= KeyCount)
        return false;

    return _released[index];
}

bool Input::mouseButtonDown(MouseButton button) const
{
    const auto index = mouseButtonIndex(button);

    if (index >= _mouseButtons.size())
        return false;

    return _mouseButtons[index];
}

bool Input::mouseButtonWasPressed(MouseButton button) const
{
    const auto index = mouseButtonIndex(button);

    if (index >= _mousePressed.size())
        return false;

    return _mousePressed[index];
}

bool Input::mouseButtonWasReleased(MouseButton button) const
{
    const auto index = mouseButtonIndex(button);

    if (index >= _mouseReleased.size())
        return false;

    return _mouseReleased[index];
}

float Input::mouseX() const
{
    return _mouseX;
}

float Input::mouseY() const
{
    return _mouseY;
}

float Input::mouseDeltaX() const
{
    return _mouseDeltaX;
}

float Input::mouseDeltaY() const
{
    return _mouseDeltaY;
}

void Input::beginFrame()
{
    _pressed.fill(false);
    _released.fill(false);

    _mousePressed.fill(false);
    _mouseReleased.fill(false);

    _mouseDeltaX = 0.0f;
    _mouseDeltaY = 0.0f;
}

void Input::processEvent(const SDL_Event& event)
{
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN:
        {
            const auto key = static_cast<std::size_t>(event.key.scancode);

            if (key >= KeyCount)
                break;

            // Ignore key-repeat events. A key is only "pressed"
            // on the initial KEY_DOWN event.
            if (!event.key.repeat) {
                _pressed[key] = true;
            }

            _keys[key] = true;
            break;
        }

        case SDL_EVENT_KEY_UP:
        {
            const auto key = static_cast<std::size_t>(event.key.scancode);

            if (key >= KeyCount)
                break;

            _keys[key] = false;
            _released[key] = true;
            break;
        }

        case SDL_EVENT_MOUSE_MOTION:
        {
            _mouseX = event.motion.x;
            _mouseY = event.motion.y;

            _mouseDeltaX += event.motion.xrel;
            _mouseDeltaY += event.motion.yrel;
            break;
        }

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        {
            std::size_t button;

            switch (event.button.button) {
                case SDL_BUTTON_LEFT:
                    button = mouseButtonIndex(MouseButton::Left);
                    break;

                case SDL_BUTTON_MIDDLE:
                    button = mouseButtonIndex(MouseButton::Middle);
                    break;

                case SDL_BUTTON_RIGHT:
                    button = mouseButtonIndex(MouseButton::Right);
                    break;

                case SDL_BUTTON_X1:
                    button = mouseButtonIndex(MouseButton::X1);
                    break;

                case SDL_BUTTON_X2:
                    button = mouseButtonIndex(MouseButton::X2);
                    break;

                default:
                    return;
            }

            _mouseButtons[button] = true;
            _mousePressed[button] = true;
            break;
        }

        case SDL_EVENT_MOUSE_BUTTON_UP:
        {
            std::size_t button;

            switch (event.button.button) {
                case SDL_BUTTON_LEFT:
                    button = mouseButtonIndex(MouseButton::Left);
                    break;

                case SDL_BUTTON_MIDDLE:
                    button = mouseButtonIndex(MouseButton::Middle);
                    break;

                case SDL_BUTTON_RIGHT:
                    button = mouseButtonIndex(MouseButton::Right);
                    break;

                case SDL_BUTTON_X1:
                    button = mouseButtonIndex(MouseButton::X1);
                    break;

                case SDL_BUTTON_X2:
                    button = mouseButtonIndex(MouseButton::X2);
                    break;

                default:
                    return;
            }

            _mouseButtons[button] = false;
            _mouseReleased[button] = true;
            break;
        }

        default:
            break;
    }
}

QTIP_CODE_END
