#include "Button.h"

Button::Button(float x, float y, float width, float height, std::string label, std::function<void()> onClick)
    : bounds({x, y, width, height}), label(label), onClick(onClick), isHovered(false), isClicked(false) {
}

void Button::Render() {
    // Draw button background based on state
    Color buttonColor = GRAY;
    Color textColor = BLACK;
    
    if (isClicked) {
        buttonColor = DARKGRAY;
        textColor = WHITE;
    } else if (isHovered) {
        buttonColor = LIGHTGRAY;
        textColor = BLACK;
    }
    
    // Draw button rectangle
    DrawRectangleRec(bounds, buttonColor);
    DrawRectangleLinesEx(bounds, 2, BLACK);
    
    // Draw button text centered
    int textWidth = MeasureText(label.c_str(), 20);
    int textX = bounds.x + (bounds.width - textWidth) / 2;
    int textY = bounds.y + (bounds.height - 20) / 2;
    
    DrawText(label.c_str(), textX, textY, 20, textColor);
}

void Button::Update() {
    Vector2 mousePos = GetMousePosition();
    
    // Check if mouse is hovering over button
    isHovered = CheckCollisionPointRec(mousePos, bounds);
    
    // Check if button is clicked
    if (isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        isClicked = true;
        if (onClick) {
            onClick(); // Call the callback function
        }
    } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        isClicked = false;
    }
}
