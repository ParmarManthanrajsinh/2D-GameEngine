# 2D Game Engine - API Reference

**Version:** 1.0  
**Last Updated:** January 2025

This document provides comprehensive API documentation for the 2D Game Engine, covering all classes, methods, and interfaces available to developers.

---

## Table of Contents

1. [Core Classes](#core-classes)
2. [Game Development](#game-development)
3. [Map System](#map-system)
4. [Configuration](#configuration)
5. [Hot-Reload System](#hot-reload-system)
6. [Particle System](#particle-system)
7. [Editor Integration](#editor-integration)
8. [Examples](#examples)

---

## Core Classes

### GameEngine

The main engine class that manages window creation, map rendering, and the core game loop.

```cpp
class GameEngine
{
public:
    GameEngine();
    ~GameEngine();
    
    // Window Management
    void LaunchWindow(int width, int height, const char* title);
    void LaunchWindow(const t_WindowConfig& config);
    void ToggleFullscreen();
    void SetWindowMode(bool fullscreen);
    
    // Map Management
    void SetMap(std::unique_ptr<GameMap> game_map);
    void DrawMap() const;
    void UpdateMap(float delta_time) const;
    void ResetMap();
    
    // MapManager Integration
    void SetMapManager(std::unique_ptr<MapManager> map_manager);
    MapManager* GetMapManager() const;
    bool b_HasMapManager() const;
};
```

#### Methods

**`LaunchWindow(int width, int height, const char* title)`**
- **Purpose**: Initialize and create the game window with specified dimensions
- **Parameters**: 
  - `width`: Window width in pixels
  - `height`: Window height in pixels  
  - `title`: Window title string
- **Usage**: `engine.LaunchWindow(1280, 720, "My Game");`

**`LaunchWindow(const t_WindowConfig& config)`**
- **Purpose**: Initialize window using configuration struct
- **Parameters**: `config` - Window configuration containing all settings
- **Usage**: 
```cpp
t_WindowConfig config;
config.width = 1920;
config.height = 1080;
config.b_Fullscreen = true;
engine.LaunchWindow(config);
```

**`SetMap(std::unique_ptr<GameMap> game_map)`**
- **Purpose**: Set the active game map/scene
- **Parameters**: `game_map` - Unique pointer to GameMap instance
- **Note**: Takes ownership of the map object

**`SetMapManager(std::unique_ptr<MapManager> map_manager)`**
- **Purpose**: Set up advanced map management system
- **Parameters**: `map_manager` - Unique pointer to MapManager instance
- **Note**: Enables hot-swapping between multiple maps

---

## Game Development

### GameMap

Base class for all game scenes/levels. Inherit from this to create your game content.

```cpp
class GameMap
{
protected:
    std::string m_MapName;
    float m_SceneWidth = 0.0f;   
    float m_SceneHeight = 0.0f;  
    std::function<void(std::string_view, bool)> m_TransitionCallback;

public:
    GameMap(); 
    GameMap(const std::string& map_name);
    virtual ~GameMap() = default;  

    // Core Methods - Override these in your maps
    virtual void Initialize();
    virtual void Update(float delta_time);
    virtual void Draw();
    
    // Utility Methods
    void SetMapName(const std::string& map_name);
    std::string GetMapName() const;
    void SetSceneBounds(float width, float height);
    Vector2 GetSceneBounds() const;
    
    // Map Transition
    void SetTransitionCallback(std::function<void(std::string_view, bool)> cb);

protected:
    void RequestGotoMap(std::string_view map_id, bool force_reload = false);
};
```

#### Creating Custom Maps

**Basic Map Example:**
```cpp
#include "../Engine/GameMap.h"

class MyLevel : public GameMap
{
private:
    Vector2 m_PlayerPos{400.0f, 300.0f};
    float m_PlayerSpeed = 200.0f;

public:
    MyLevel() : GameMap("MyLevel") {}
    
    void Initialize() override 
    {
        // Initialize your level data
        SetSceneBounds(800.0f, 600.0f);
    }
    
    void Update(float delta_time) override 
    {
        // Handle input and game logic
        if (IsKeyDown(KEY_RIGHT)) 
            m_PlayerPos.x += m_PlayerSpeed * delta_time;
        if (IsKeyDown(KEY_LEFT)) 
            m_PlayerPos.x -= m_PlayerSpeed * delta_time;
            
        // Transition to another map
        if (IsKeyPressed(KEY_ENTER))
            RequestGotoMap("NextLevel");
    }
    
    void Draw() override 
    {
        // Render your game content
        DrawCircleV(m_PlayerPos, 25.0f, RED);
        DrawText("Press ENTER for next level", 10, 10, 20, BLACK);
    }
};
```

#### Virtual Methods

**`Initialize()`**
- **Purpose**: Called once when the map is first created
- **Usage**: Set up initial game state, load assets, initialize variables

**`Update(float delta_time)`**
- **Purpose**: Called every frame to update game logic
- **Parameters**: `delta_time` - Time elapsed since last frame in seconds
- **Usage**: Handle input, update physics, check collisions

**`Draw()`**
- **Purpose**: Called every frame to render game content
- **Usage**: Draw sprites, UI elements, effects using Raylib functions

---

## Map System

### MapManager

Advanced map management system supporting registration, switching, and hot-reloading of multiple maps.

```cpp
class MapManager : public GameMap
{
public:
    MapManager();
    ~MapManager() override;
    
    // Map Registration
    template<typename T>
    void RegisterMap(const std::string& map_id, const std::string& description = "");
    
    // Map Navigation
    bool b_GotoMap(const std::string& map_id, bool force_reload = false);
    bool b_IsCurrentMap(const std::string& map_id) const;
    bool b_IsMapRegistered(const std::string& map_id) const;
    bool b_ReloadCurrentMap();
    
    // Information
    const std::string& GetCurrentMapId() const;
    std::vector<std::string> GetAvailableMaps() const;
    std::string GetDebugInfo() const;
    
    // Management
    void UnloadCurrentMap();
};
```

#### Map Registration

**Template Registration:**
```cpp
// Register maps by type
manager->RegisterMap<Level1>("level_1", "First level with basic gameplay");
manager->RegisterMap<Level2>("level_2", "Second level with advanced mechanics");
manager->RegisterMap<BossLevel>("boss", "Final boss battle");
```

**Helper Templates:**
```cpp
// Alternative registration syntax
RegisterMap<Level1>(*manager, "level_1");
RegisterMapAs<BossLevel>(*manager, "boss_fight", "Epic boss battle");
```

#### Map Navigation

**Switching Maps:**
```cpp
// Go to specific map
if (manager->b_GotoMap("level_2")) {
    std::cout << "Successfully switched to Level 2" << std::endl;
}

// Force reload current map (useful for testing)
manager->b_ReloadCurrentMap();

// Check current map
if (manager->b_IsCurrentMap("boss_fight")) {
    // Boss-specific logic
}
```

#### Complete MapManager Example

```cpp
// GameLogic/RootManager.cpp
#include "../Engine/MapManager.h"
#include "Level1.h"
#include "Level2.h"

extern "C" __declspec(dllexport) GameMap* CreateGameMap()
{
    MapManager* manager = new MapManager();
    
    // Register all your maps
    manager->RegisterMap<Level1>("level_1", "Tutorial level");
    manager->RegisterMap<Level2>("level_2", "Advanced level");
    
    // Load the first map
    auto maps = manager->GetAvailableMaps();
    if (!maps.empty()) {
        manager->b_GotoMap(maps[0]);
    }
    
    return manager;
}
```

---

## Configuration

### GameConfig

Singleton configuration management system for window settings and game options.

```cpp
class GameConfig 
{
public:
    static GameConfig& GetInstance();
    bool m_bLoadFromFile(const std::string& config_path = "game_config.ini");
    bool m_bSaveToFile(const std::string& config_path = "game_config.ini") const;
    
    t_WindowConfig& GetWindowConfig();
    const t_WindowConfig& GetWindowConfig() const;
};
```

### t_WindowConfig

Window configuration structure containing all display settings.

```cpp
struct t_WindowConfig 
{
    int width = 1280;              // Window width in pixels
    int height = 720;              // Window height in pixels
    bool b_Fullscreen = false;     // Start in fullscreen mode
    bool b_Resizable = true;       // Allow window resizing
    bool b_Vsync = true;          // Enable vertical sync
    int target_fps = 60;          // Target frame rate (0 = unlimited)
    std::string title = "My Game"; // Window title
};
```

#### Configuration File Format

The engine uses `game_config.ini` for persistent settings:

```ini
# Game Configuration File
# Window Settings
width=1280
height=720
b_Fullscreen=false
b_Resizable=true
b_Vsync=true
target_fps=60
title=MyGame
```

#### Usage Examples

**Loading Configuration:**
```cpp
GameConfig& config = GameConfig::GetInstance();
if (config.m_bLoadFromFile("my_config.ini")) {
    std::cout << "Configuration loaded successfully" << std::endl;
}

// Use configuration with engine
GameEngine engine;
engine.LaunchWindow(config.GetWindowConfig());
```

**Modifying Configuration:**
```cpp
GameConfig& config = GameConfig::GetInstance();
config.GetWindowConfig().width = 1920;
config.GetWindowConfig().height = 1080;
config.GetWindowConfig().b_Fullscreen = true;

// Save changes
config.m_bSaveToFile("game_config.ini");
```

**Runtime Window Control:**
```cpp
// Toggle fullscreen (Alt+Enter is handled automatically)
engine.ToggleFullscreen();

// Set specific mode
engine.SetWindowMode(true);  // fullscreen
engine.SetWindowMode(false); // windowed
```

---

## Hot-Reload System

### DllLoader

Low-level DLL management for hot-reloading game logic without restarting the application.

```cpp
struct DllHandle 
{
    void* handle;           // Windows HANDLE to loaded DLL
    std::string shadow_path; // Path to shadow-copied DLL file
};

// Core Functions
DllHandle LoadDll(const char* path);
void UnloadDll(DllHandle dll);
void* GetDllSymbol(DllHandle dll, const char* symbol_name);
```

#### How Hot-Reload Works

1. **Shadow Copying**: Creates a copy of `GameLogic.dll` with a unique name
2. **Load Shadow**: Loads the shadow copy, leaving original file unlocked
3. **Symbol Resolution**: Extracts the `CreateGameMap` function pointer
4. **File Watching**: Monitors original DLL for changes
5. **Automatic Reload**: Detects changes and reloads within ~0.5 seconds

#### DLL Entry Point

Your game logic DLL must export the `CreateGameMap` function:

```cpp
// GameLogic/RootManager.cpp
extern "C" __declspec(dllexport) GameMap* CreateGameMap()
{
    // Return your root map or MapManager instance
    return new MyGameMap();
}
```

#### Development Workflow

```bash
# 1. Start the editor
main.exe

# 2. Edit your game code in GameLogic/
# (Edit Level1.cpp, add new features, etc.)

# 3. Rebuild GameLogic while editor is running
cmake --build out/build/x64-debug --config Debug --target GameLogic

# 4. Engine automatically reloads in ~0.5 seconds
# OR click the "Restart" button in the editor
```

**Important Notes:**
- Game state resets on reload (this is intentional for testing)
- Only `GameLogic.dll` can be hot-reloaded
- To rebuild `main.exe`, close the editor first (LNK1168 error)
- Shadow files are automatically cleaned up

---

## Particle System

### FireParticle

Advanced particle system for creating visual effects like fire, smoke, explosions, and more.

```cpp
class FireParticle
{
public:
    Vector2 position;  // Emitter position
    
    FireParticle();
    FireParticle(float x, float y);
    ~FireParticle();
    
    void Update(float dt);
    void Draw();
    
    // Texture Management
    bool b_LoadTexture(const char* filename);
    void UnloadTexture();
    void SetUseTexture(bool use);
    bool b_IsUsingTexture() const;
};
```

#### Particle Configuration

```cpp
// Emitter Types
enum EmitterType {
    POINT,      // Single point emission
    LINE,       // Linear emission
    CIRCLE,     // Circular area emission  
    RECTANGLE   // Rectangular area emission
};

// Particle Shapes
enum ParticleType {
    CIRCULER,   // Round particles
    SQUARE,     // Square particles
    TRIANGLE,   // Triangle particles
    K_CHAR      // Custom K-shaped particles
};
```

#### Particle Properties

Each particle has the following properties (configurable via member variables):

- **Position & Motion**: `velocity`, `velocity_variation`, `acceleration`
- **Appearance**: `start_color`, `end_color`, `min_size`, `max_size`
- **Lifetime**: `min_life`, `max_life`, `emission_rate`
- **Rotation**: `rotation_speed`
- **Emitter Shape**: `line_length`, `circle_radius`, `rect_size`

#### Usage Examples

**Basic Fire Effect:**
```cpp
// Create fire particle system
FireParticle fire(400.0f, 500.0f);

// In your Update() method
fire.Update(deltaTime);

// In your Draw() method
fire.Draw();
```

**Advanced Configuration:**
```cpp
FireParticle explosion(playerPos.x, playerPos.y);

// Configure as explosion
explosion.e_EmitterType = CIRCLE;
explosion.circle_radius = 50.0f;
explosion.start_color = {255, 255, 0, 255};  // Yellow
explosion.end_color = {255, 0, 0, 255};      // Red
explosion.velocity = {0.0f, -100.0f};
explosion.velocity_variation = {200.0f, 200.0f};
explosion.min_life = 0.5f;
explosion.max_life = 2.0f;
explosion.emission_rate = 200.0f;
```

**Texture-Based Particles:**
```cpp
FireParticle magic(wizardPos.x, wizardPos.y);

// Load particle texture
if (magic.b_LoadTexture("Assets/star.png")) {
    magic.SetUseTexture(true);
    magic.e_EmitterType = CIRCLE;
    magic.circle_radius = 20.0f;
}
```

#### Particle System in Game Maps

```cpp
class MyLevel : public GameMap
{
private:
    std::vector<FireParticle> m_Effects;
    
public:
    void Initialize() override 
    {
        // Create campfire effect
        FireParticle campfire(300.0f, 400.0f);
        campfire.start_color = {255, 100, 0, 255};
        campfire.end_color = {255, 0, 0, 255};
        m_Effects.push_back(campfire);
    }
    
    void Update(float delta_time) override 
    {
        // Update all particle systems
        for (auto& effect : m_Effects) {
            effect.Update(delta_time);
        }
    }
    
    void Draw() override 
    {
        // Draw all particle systems
        for (auto& effect : m_Effects) {
            effect.Draw();
        }
    }
};
```

---

## Editor Integration

### GameEditor

The ImGui-based development interface providing hot-reload controls, map selection, and export functionality.

```cpp
class GameEditor
{
public:
    GameEditor();
    ~GameEditor();
    
    void Init(int width, int height, std::string_view title);
    void Run();
    
    // Hot-Reload Management
    bool b_LoadGameLogic(const char* dllPath);
    bool b_ReloadGameLogic();
    
private:
    GameEngine m_GameEngine;
    bool b_IsPlaying;
    std::atomic<bool> b_IsCompiling;
};
```

#### Editor Features

**Toolbar Controls:**
- ▶️ **Play/Pause**: Start/stop game simulation
- 🔄 **Restart**: Force reload of GameLogic.dll
- 🔨 **Compile**: Rebuild GameLogic.dll
- 🧹 **Clean**: Clean build artifacts

**Map Selection Panel:**
- View all registered maps
- Switch between maps at runtime
- Display map descriptions and status

**Export Panel:**
- Configure window settings for distribution
- Set resolution presets (720p, 1080p, 4K, etc.)
- Choose FPS targets (30, 60, 120, 144, 240, unlimited)
- Export standalone game executable

**File Explorer:**
- Browse project assets
- View file tree structure
- Access configuration files

#### Editor Usage

The editor automatically:
- Detects file changes in `GameLogic.dll`
- Reloads modified code within ~0.5 seconds
- Preserves editor state across reloads
- Provides visual feedback for compilation status

---

## Examples

### Complete Level Implementation

```cpp
// GameLogic/PlatformerLevel.h
#pragma once
#include "../Engine/GameMap.h"
#include "FireParticle.h"
#include <vector>

class PlatformerLevel : public GameMap
{
private:
    // Player
    Vector2 m_PlayerPos;
    Vector2 m_PlayerVel;
    bool m_bIsGrounded;
    
    // Level geometry
    std::vector<Rectangle> m_Platforms;
    Rectangle m_FinishZone;
    
    // Effects
    std::vector<FireParticle> m_Torches;
    
    // Constants
    static constexpr float PLAYER_SIZE = 32.0f;
    static constexpr float GRAVITY = 1500.0f;
    static constexpr float JUMP_FORCE = -650.0f;
    static constexpr float MOVE_SPEED = 400.0f;

public:
    PlatformerLevel() : GameMap("PlatformerLevel") {}
    
    void Initialize() override;
    void Update(float delta_time) override;
    void Draw() override;
    
private:
    void UpdatePlayer(float delta_time);
    void CheckCollisions();
    bool IsColliding(Rectangle rect1, Rectangle rect2);
};

// PlatformerLevel.cpp
#include "PlatformerLevel.h"

void PlatformerLevel::Initialize()
{
    // Set scene bounds
    SetSceneBounds(1280.0f, 720.0f);
    
    // Initialize player
    m_PlayerPos = {100.0f, 300.0f};
    m_PlayerVel = {0.0f, 0.0f};
    m_bIsGrounded = false;
    
    // Create platforms
    m_Platforms = {
        {0.0f, 650.0f, 1280.0f, 70.0f},      // Ground
        {300.0f, 500.0f, 200.0f, 20.0f},     // Platform 1
        {600.0f, 350.0f, 200.0f, 20.0f},     // Platform 2
        {900.0f, 200.0f, 200.0f, 20.0f}      // Platform 3
    };
    
    // Set finish zone
    m_FinishZone = {1150.0f, 100.0f, 100.0f, 100.0f};
    
    // Create torch effects
    FireParticle torch1(150.0f, 600.0f);
    torch1.start_color = {255, 150, 0, 255};
    torch1.end_color = {255, 50, 0, 255};
    torch1.circle_radius = 15.0f;
    m_Torches.push_back(torch1);
    
    FireParticle torch2(1100.0f, 150.0f);
    torch2.start_color = {255, 150, 0, 255};
    torch2.end_color = {255, 50, 0, 255};
    torch2.circle_radius = 15.0f;
    m_Torches.push_back(torch2);
}

void PlatformerLevel::Update(float delta_time)
{
    UpdatePlayer(delta_time);
    CheckCollisions();
    
    // Update effects
    for (auto& torch : m_Torches) {
        torch.Update(delta_time);
    }
    
    // Check win condition
    Rectangle playerRect = {
        m_PlayerPos.x - PLAYER_SIZE/2, 
        m_PlayerPos.y - PLAYER_SIZE/2,
        PLAYER_SIZE, PLAYER_SIZE
    };
    
    if (IsColliding(playerRect, m_FinishZone)) {
        RequestGotoMap("NextLevel");
    }
    
    // Reset on fall
    if (m_PlayerPos.y > 800.0f) {
        m_PlayerPos = {100.0f, 300.0f};
        m_PlayerVel = {0.0f, 0.0f};
    }
}

void PlatformerLevel::UpdatePlayer(float delta_time)
{
    // Horizontal movement
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        m_PlayerVel.x = -MOVE_SPEED;
    } else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        m_PlayerVel.x = MOVE_SPEED;
    } else {
        m_PlayerVel.x = 0.0f;
    }
    
    // Jumping
    if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W)) && m_bIsGrounded) {
        m_PlayerVel.y = JUMP_FORCE;
        m_bIsGrounded = false;
    }
    
    // Apply gravity
    if (!m_bIsGrounded) {
        m_PlayerVel.y += GRAVITY * delta_time;
    }
    
    // Update position
    m_PlayerPos.x += m_PlayerVel.x * delta_time;
    m_PlayerPos.y += m_PlayerVel.y * delta_time;
}

void PlatformerLevel::CheckCollisions()
{
    Rectangle playerRect = {
        m_PlayerPos.x - PLAYER_SIZE/2, 
        m_PlayerPos.y - PLAYER_SIZE/2,
        PLAYER_SIZE, PLAYER_SIZE
    };
    
    m_bIsGrounded = false;
    
    for (const auto& platform : m_Platforms) {
        if (IsColliding(playerRect, platform)) {
            // Landing on top of platform
            if (m_PlayerVel.y > 0 && 
                m_PlayerPos.y < platform.y + PLAYER_SIZE/2) {
                m_PlayerPos.y = platform.y - PLAYER_SIZE/2;
                m_PlayerVel.y = 0.0f;
                m_bIsGrounded = true;
            }
        }
    }
}

void PlatformerLevel::Draw()
{
    // Clear background
    ClearBackground(SKYBLUE);
    
    // Draw platforms
    for (const auto& platform : m_Platforms) {
        DrawRectangleRec(platform, DARKGRAY);
        DrawRectangleLinesEx(platform, 2.0f, BLACK);
    }
    
    // Draw finish zone
    DrawRectangleRec(m_FinishZone, GOLD);
    DrawText("FINISH", 
             m_FinishZone.x + 10, m_FinishZone.y + 40, 
             20, BLACK);
    
    // Draw player
    DrawCircleV(m_PlayerPos, PLAYER_SIZE/2, RED);
    DrawCircleLinesV(m_PlayerPos, PLAYER_SIZE/2, DARKRED);
    
    // Draw torch effects
    for (auto& torch : m_Torches) {
        torch.Draw();
    }
    
    // Draw UI
    DrawText("Use WASD/Arrow Keys to move, SPACE to jump", 
             10, 10, 20, BLACK);
    DrawText(TextFormat("Position: %.1f, %.1f", m_PlayerPos.x, m_PlayerPos.y),
             10, 40, 16, BLACK);
}

bool PlatformerLevel::IsColliding(Rectangle rect1, Rectangle rect2)
{
    return CheckCollisionRecs(rect1, rect2);
}
```

### Multi-Map Game Setup

```cpp
// GameLogic/RootManager.cpp
#include "../Engine/MapManager.h"
#include "MainMenu.h"
#include "PlatformerLevel.h"
#include "BossLevel.h"

extern "C" __declspec(dllexport) GameMap* CreateGameMap()
{
    MapManager* manager = new MapManager();
    
    // Register all game maps
    manager->RegisterMap<MainMenu>("main_menu", "Main menu with options");
    manager->RegisterMap<PlatformerLevel>("level_1", "Tutorial platforming level");
    manager->RegisterMap<BossLevel>("boss", "Final boss battle");
    
    // Start at main menu
    manager->b_GotoMap("main_menu");
    
    return manager;
}
```

---

## Best Practices

### Development Workflow
1. **Use Hot-Reload**: Edit code while the engine runs for rapid iteration
2. **Start Small**: Begin with simple maps and add complexity gradually
3. **Test Frequently**: Use the restart button to test changes immediately
4. **Organize Maps**: Use descriptive names and register maps systematically

### Performance Tips
1. **Limit Particles**: Keep particle counts reasonable for target hardware
2. **Optimize Draw Calls**: Batch similar rendering operations
3. **Use Delta Time**: Always scale movement by `delta_time` for frame-rate independence
4. **Clean Up Resources**: Unload textures and resources when changing maps

### Code Organization
1. **Follow Naming Conventions**: Use the established naming patterns
2. **Separate Concerns**: Keep game logic, rendering, and input handling separate
3. **Document Maps**: Provide descriptions when registering maps
4. **Handle Errors**: Check return values and provide fallbacks

### Distribution
1. **Test Exports**: Always test exported games on clean systems
2. **Include Assets**: Ensure all required assets are included in distribution
3. **Configure Settings**: Provide reasonable default configurations
4. **Document Controls**: Include control instructions in your game

---

**For additional help and examples, see the other documentation files in the `Documentation/` folder.**