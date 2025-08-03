#include "Button.h"

Button::Button(float x, float y, float width, float height, std::string label, std::function<void()> onClick)
    : bounds({x, y, width, height}), label(label), on_click(onClick), b_IsHovered(false), b_IsClicked(false)
{
}

void Button::Render()
{
    // Draw button background based on state
    Color buttonColor = GRAY;
    Color textColor = BLACK;

    if (b_IsClicked)
    {
        buttonColor = DARKGRAY;
        textColor = WHITE;
    }
    else if (b_IsHovered)
    {
        buttonColor = LIGHTGRAY;
        textColor = BLACK;
    }

    // Draw button rectangle
    DrawRectangleRec(bounds, buttonColor);
    DrawRectangleLinesEx(bounds, 2, BLACK);

    // Draw button text centered
    int textWidth = MeasureText(label.c_str(), 20);
    int textX = static_cast<int>(bounds.x + (bounds.width - static_cast<float>(textWidth)) / 2.0f);
    int textY = static_cast<int>(bounds.y + (bounds.height - 20) / 2.0f);

    DrawText(label.c_str(), textX, textY, 20, textColor);
}

void Button::Update()
{
    Vector2 mousePos = GetMousePosition();

    // Check if mouse is hovering over button
    b_IsHovered = CheckCollisionPointRec(mousePos, bounds);

    // Check if button is clicked
    if (b_IsHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        b_IsClicked = true;
        if (on_click)
        {
            on_click(); // Call the callback function
        }
    }
    else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        b_IsClicked = false;
    }
}
