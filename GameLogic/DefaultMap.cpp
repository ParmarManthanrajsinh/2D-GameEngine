#include "DefaultMap.h"

namespace Colors {
    constexpr Color SoftBackground{ 25, 30, 40, 255 };
    constexpr Color HeaderBackground{ 35, 40, 50, 255 };
    constexpr Color HeaderText{ 255, 255, 255, 255 };
    constexpr Color StepHeader{ 100, 200, 255, 255 };
    constexpr Color CodeSnippet{ 150, 255, 150, 255 };
    constexpr Color CodeImplementation{ 200, 200, 200, 255 };
    constexpr Color TipsTitle{ 255, 200, 100, 255 };
    constexpr Color TipsText{ 200, 200, 200, 255 };
    constexpr Color AnimatedIndicator{ 255, 200, 100, 150 };
    constexpr Color FooterText{ 150, 150, 150, 255 };
}

DefaultMap::DefaultMap()
    : m_TimeAlive(0.0f)
{
}

void DefaultMap::Initialize()
{
    SetMapName("Engine Tutorial");
    m_TimeAlive = 0.0f;
    std::cout << "[DefaultMap] Engine tutorial initialized" << std::endl;
    center_x = static_cast<int>(GetSceneBounds().x / 2);
}

void DefaultMap::Update(float dt)
{
    m_TimeAlive += dt;
}

void DefaultMap::Draw()
{
    Vector2 screen_size = GetSceneBounds();

    // Soft background
    DrawRectangle(0, 0, static_cast<int>(screen_size.x), static_cast<int>(screen_size.y), Colors::SoftBackground);

    // Header section
    DrawRectangle(0, 0, static_cast<int>(screen_size.x), 100, Colors::HeaderBackground);

    const char* title = "Welcome to Your 2D Game Engine";
    int title_width = MeasureText(title, 32);
    DrawText(title, center_x - title_width / 2, 35, 32, Colors::HeaderText);

    content_y = 140;

    // Step 1
    DrawText("Step 1: Create Your Map", margin_left, content_y, 24, Colors::StepHeader);
    DrawText("class MyMap : public GameMap {", margin_left + 20, content_y + 30, 16, Colors::CodeSnippet);
    DrawText("    // Your map implementation", margin_left + 20, content_y + 50, 16, Colors::CodeImplementation);
    DrawText("};", margin_left + 20, content_y + 70, 16, Colors::CodeSnippet);

    content_y += step_spacing;

    // Step 2
    DrawText("Step 2: Register Your Map", margin_left, content_y, 24, Colors::StepHeader);
    DrawText("mapManager.RegisterMap<MyMap>(\"my_map\");", margin_left + 20, content_y + 30, 16, Colors::CodeSnippet);

    content_y += step_spacing;

    // Step 3
    DrawText("Step 3: Switch to Your Map", margin_left, content_y, 24, Colors::StepHeader);
    DrawText("mapManager.GoToMap(\"my_map\");", margin_left + 20, content_y + 30, 16, Colors::CodeSnippet);

    // Tips section
    content_y += 100;
    DrawText("Quick Tips:", margin_left, content_y, 20, Colors::TipsTitle);
    DrawText("• Override Initialize() to setup your map", margin_left + 20, content_y + 30, 16, Colors::TipsText);
    DrawText("• Override Update() for game logic", margin_left + 20, content_y + 50, 16, Colors::TipsText);
    DrawText("• Override Draw() to render your game", margin_left + 20, content_y + 70, 16, Colors::TipsText);

    // Animated indicator
    float bounce = sin(m_TimeAlive * 3.0f) * 5;
    DrawCircle(50, static_cast<int>(content_y + 40 + bounce), 8, Colors::AnimatedIndicator);

    // Footer
    const char* footer = "Create your first map to get started!";
    int footerWidth = MeasureText(footer, 18);
    DrawText(footer, center_x - footerWidth / 2, static_cast<int>(screen_size.y) - 60, 18, Colors::FooterText);
}