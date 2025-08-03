#pragma once

#include <raylib.h>
#include <string>
#include <functional>

class Button
{
public:
    Button(float x, float y, float width, float height, std::string label, std::function<void()> onClick);

    void Render();

    // Update the button (check for clicks)
    void Update();

private:
    Rectangle bounds;              // Button bounds
    std::string label;             // Button label
    std::function<void()> on_click; // Click callback

    // Button states
    bool b_IsHovered;
    bool b_IsClicked;
};