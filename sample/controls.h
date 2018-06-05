#pragma once


#include <glm/glm.hpp>

class Controls 
{
public:
    enum Button
    {
        MOUSE_LEFT,
        MOUSE_RIGHT
    };

    void setCursorPosition(double xpos, double ypos);
    void setMouseButton(int button, int action, int mods);

    bool buttonPressed(Button b);
    bool buttonHeld(Button b);
    bool buttonReleased(Button b);

    const glm::vec2& mousePos() { return _mousePosition; }

    glm::vec2 deltaMousePos();

    void updateStates();

private:
    uint32_t _pressedFlags;
    uint32_t _holdFlags;
    uint32_t _releaseFlags;

    glm::vec2 _mousePosition;
    glm::vec2 _lastMousePosition;
};