# RayWaves Engine - API Reference

Essential classes and methods for RayWaves game development.

## Quick Start

### 1. Create a Map
```cpp
#include "../Engine/GameMap.h"

class MyLevel : public GameMap {
private:
    Vector2 m_PlayerPos{400.0f, 300.0f};
    
public:
    MyLevel() : GameMap("MyLevel") {}
    
    void Update(float delta_time) override {
        if (IsKeyDown(KEY_RIGHT)) m_PlayerPos.x += 200.0f * delta_time;
        if (IsKeyPressed(KEY_ENTER)) RequestGotoMap("NextLevel");
    }
    
    void Draw() override {
        DrawCircleV(m_PlayerPos, 25.0f, RED);
    }
};
```

### 2. Register Maps
```cpp
// GameLogic/RootManager.cpp
extern "C" __declspec(dllexport) GameMap* CreateGameMap() {
    MapManager* manager = new MapManager();
    manager->RegisterMap<MyLevel>("MyLevel");
    manager->b_GotoMap("MyLevel");
    return manager;
}
```

## GameMap - Base Class for Levels

```cpp
class GameMap {
public:
    virtual void Update(float delta_time) = 0;  // Game logic
    virtual void Draw() = 0;                    // Rendering
    virtual void Initialize() {}                // Setup (optional)
    virtual void Cleanup() {}                   // Cleanup (optional)
    
    // Map transitions from within your map
    void RequestGotoMap(std::string_view map_id, bool force_reload = false);
    std::string GetMapName() const;
};
```

## MapManager - Level Management

```cpp
class MapManager : public GameMap {
public:
    template<typename T>
    void RegisterMap(const std::string& map_id);
    
    bool b_GotoMap(const std::string& map_id, bool force_reload = false);
    bool b_IsCurrentMap(const std::string& map_id) const;
    std::string GetCurrentMapId() const;
    std::vector<std::string> GetAvailableMaps() const;
};
```

## GameConfig - Settings Management

```cpp
class GameConfig {
public:
    static GameConfig& GetInstance();
    bool m_bLoadFromFile(const std::string& config_path = "config.ini");
    bool m_bSaveToFile(const std::string& config_path = "config.ini") const;
    t_WindowConfig& GetWindowConfig();
};

struct t_WindowConfig {
    int width = 1280;
    int height = 720;
    bool b_Fullscreen = false;
    bool b_Resizable = true;
    bool b_Vsync = true;
    int target_fps = 60;
    std::string title = "RayWaves Game";
};
```

### Config File Format
```ini
width=1280
height=720
b_Fullscreen=false
b_Resizable=true
b_Vsync=true
target_fps=60
title=My Game
```

## Common Patterns

### Player Movement
```cpp
void Update(float delta_time) override {
    const float SPEED = 300.0f;
    
    if (IsKeyDown(KEY_RIGHT)) m_PlayerPos.x += SPEED * delta_time;
    if (IsKeyDown(KEY_LEFT))  m_PlayerPos.x -= SPEED * delta_time;
    if (IsKeyDown(KEY_UP))    m_PlayerPos.y -= SPEED * delta_time;
    if (IsKeyDown(KEY_DOWN))  m_PlayerPos.y += SPEED * delta_time;
}
```

### Level Transitions
```cpp
void Update(float delta_time) override {
    // Win condition
    if (CheckCollisionRecs(playerRect, finishRect)) {
        RequestGotoMap("NextLevel");
    }
    
    // Restart level
    if (IsKeyPressed(KEY_R)) {
        RequestGotoMap(GetMapName(), true);  // force_reload = true
    }
}
```

### Configuration Loading
```cpp
void Initialize() override {
    GameConfig& config = GameConfig::GetInstance();
    if (config.m_bLoadFromFile()) {
        auto& winConfig = config.GetWindowConfig();
        std::cout << "Window: " << winConfig.width << "x" << winConfig.height << std::endl;
    }
}
```

## Tips

- Always use `delta_time` for frame-rate independent movement
- Use `RequestGotoMap()` from within maps, `b_GotoMap()` from manager
- Config files must be next to the executable in distributed games
- Hot-reload works automatically when you rebuild GameLogic.dll

---

That's it! For more examples, check the `GameLogic/` folder in your project.