#pragma once

#include <raylib.h>
#include <string>
#include <functional>

class Button {
public:
    // Constructor
    Button(float x, float y, float width, float height, std::string label, std::function<void()> onClick);
    
    // Render the button
    void Render();
    
    // Update the button (check for clicks)
    void Update();
    
private:
    Rectangle bounds;       // Button bounds
    std::string label;      // Button label
    std::function<void()> onClick; // Click callback

    // Button states
    bool isHovered;
    bool isClicked;
};
