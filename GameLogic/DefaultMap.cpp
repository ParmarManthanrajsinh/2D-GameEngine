#include "DefaultMap.h"

DefaultMap::DefaultMap()
    : m_TimeAlive(0.0f)
{
}

void DefaultMap::Initialize()
{
    SetMapName("Default Map");
    m_TimeAlive = 0.0f;
    std::cout << "[DefaultMap] Initialized as fallback map" << std::endl;
}

void DefaultMap::Update(float deltaTime)
{
    m_TimeAlive += deltaTime;
}

void DefaultMap::Draw()
{
    // Draw a simple background
    DrawRectangle(0, 0, GetSceneBounds().x, GetSceneBounds().y, DARKGRAY);
    
    // Title
    DrawText("DEFAULT MAP", 200, 100, 40, WHITE);
    
    // Helpful information for developers
    DrawText("No maps registered!", 150, 200, 25, RED);
    DrawText("Register maps using:", 100, 250, 20, LIGHTGRAY);
    DrawText("manager.RegisterMap<YourMap>(\"map_id\");", 120, 280, 18, YELLOW);
    
    // Example code
    DrawText("Example:", 100, 320, 20, LIGHTGRAY);
    DrawText("manager.RegisterMap<YourMap>(\"map_id\");", 120, 350, 16, GREEN);
    DrawText("manager.GoToMap(\"map_id\");", 120, 370, 16, GREEN);
    
    // Animated elements to show the map is alive
    int pulse = (int)(sin(m_TimeAlive * 2.0f) * 50 + 200);
    DrawCircle(400, 450, 20 + sin(m_TimeAlive) * 5, 
               Color{255, (unsigned char)pulse, (unsigned char)pulse, 100});
    
    // Footer
    DrawText("MapManager v1.0", 10, GetSceneBounds().y - 30, 16, DARKBLUE);
}