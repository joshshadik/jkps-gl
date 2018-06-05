#include "controls.h"

#include <GLFW/glfw3.h>

#include <stdio.h>

void Controls::setCursorPosition(double xpos, double ypos)
{
    _mousePosition.x = xpos;
    _mousePosition.y = ypos;

}

void Controls::setMouseButton(int button, int action, int mods)
{
    int b = -1;
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        b = 1 << (int)Button::MOUSE_LEFT;
        break;

    case GLFW_MOUSE_BUTTON_RIGHT:
        b = 1 << (int)Button::MOUSE_RIGHT;
        break;
    }


    if (b > 0)
    {
        if (action == GLFW_PRESS)
        {
            _pressedFlags |= b;
            _holdFlags &= ~b;
            _releaseFlags &= ~b;
        }
        else
        {
            _pressedFlags &= ~b;
            _holdFlags &= ~b;
            _releaseFlags |= b;
        }
    }
}

bool Controls::buttonPressed(Button b)
{
    return ((_pressedFlags >> (int)b) & 1) == 1;
}

bool Controls::buttonHeld(Button b)
{
    return ((_holdFlags >> (int)b) & 1) == 1;
}

bool Controls::buttonReleased(Button b)
{
    return ((_releaseFlags >> (int)b) & 1) == 1;
}

glm::vec2 Controls::deltaMousePos()
{
    glm::vec2 delta = _mousePosition - _lastMousePosition;
    return delta;
}

void Controls::updateStates()
{
    for (int i = 0; i <= 2; ++i)
    {
        if (buttonPressed((Button)i))
        {
            _pressedFlags &= ~(1 << i);
            _holdFlags |= (1 << i);
            _releaseFlags &= ~(1 << i);
        }
        else if (buttonReleased((Button)i))
        {
            _pressedFlags &= ~(1 << i);
            _holdFlags &= ~(1 << i);
            _releaseFlags &= ~(1 << i);
        }
    }

    _lastMousePosition = _mousePosition;
}
