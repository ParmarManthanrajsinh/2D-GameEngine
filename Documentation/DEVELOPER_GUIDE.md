# 2D Game Engine - Developer Guide

**Version:** 1.0  
**Last Updated:** January 2025

This guide provides comprehensive information for developers working with the 2D Game Engine, covering architecture, development patterns, and best practices.

---

## Table of Contents

1. [Architecture Overview](#architecture-overview)
2. [Development Setup](#development-setup)
3. [Hot-Reload Development](#hot-reload-development)
4. [Map Development Patterns](#map-development-patterns)
5. [Asset Management](#asset-management)
6. [Performance Guidelines](#performance-guidelines)
7. [Debugging and Testing](#debugging-and-testing)
8. [Distribution and Deployment](#distribution-and-deployment)

---

## Architecture Overview

### System Components

The engine follows a modular architecture with clear separation of concerns:

```
┌─────────────────────────────────────────────────────────────┐
│                        Main Application                     │
│  ┌─────────────────┐  ┌─────────────────┐  ┌──────────────┐ │
│  │   GameEditor    │  │   GameEngine    │  │  DllLoader   │ │
│  │  (ImGui UI)     │  │ (Core Runtime)  │  │ (Hot-Reload) │ │
│  └─────────────────┘  └─────────────────┘  └──────────────┘ │
└─────────────────────────────────────────────────────────────┘
                               │
                               │ Loads
                               ▼
┌─────────────────────────────────────────────────────────────┐
│                     GameLogic.dll                           │
│  ┌─────────────────┐  ┌─────────────────┐  ┌──────────────┐ │
│  │   MapManager    │  │   Custom Maps   │  │  Particles   │ │
│  │ (Map System)    │  │  (Game Logic)   │  │  (Effects)   │ │
│  └─────────────────┘  └─────────────────┘  └──────────────┘ │
└─────────────────────────────────────────────────────────────┘
                               │
                               │ Uses
                               ▼
┌─────────────────────────────────────────────────────────────┐
│                      Engine Static Lib                      │
│  ┌─────────────────┐  ┌─────────────────┐  ┌──────────────┐ │
│  │    GameMap      │  │   GameConfig    │  │    Raylib    │ │
│  │  (Base Class)   │  │ (Configuration) │  │ (Graphics)   │ │
│  └─────────────────┘  └─────────────────┘  └──────────────┘ │
└─────────────────────────────────────────────────────────────┘
```

### Data Flow

1. **Startup**: Main application initializes GameEngine and GameEditor
2. **DLL Loading**: DllLoader creates shadow copy and loads GameLogic.dll
3. **Map Creation**: Calls `CreateGameMap()` export function from DLL
4. **Game Loop**: Engine updates and renders the active map
5. **Hot-Reload**: File watcher detects changes and triggers reload
6. **Map Transitions**: MapManager handles switching between scenes

### Key Design Principles

**Hot-Reload First**: The entire architecture is designed around the ability to reload game logic without restarting the application.

**Shared Libraries**: Raylib is built as a shared library so both the main application and GameLogic.dll use the same graphics context.

**Interface Stability**: The Engine static library provides a stable interface that doesn't change during hot-reload cycles.

**Resource Management**: RAII patterns and smart pointers ensure proper cleanup during reloads.

---

## Development Setup

### Prerequisites

- **Windows 10/11**: Required for DLL hot-reload system
- **Visual Studio Build Tools**: MSVC compiler and linker
- **CMake 3.10+**: Build system management
- **Git**: Version control (recommended)

### Build Configuration

The project uses CMake with preset configurations:

```cmake
# Available presets
x64-debug    # Debug build with symbols
x64-release  # Optimized release build
```

### Project Structure

```
2D-GameEngine/
├── Engine/                 # Core engine (static library)
│   ├── GameEngine.h/cpp   # Main engine class
│   ├── GameMap.h/cpp      # Base map class
│   ├── MapManager.h/cpp   # Map management system
│   └── GameConfig.h/cpp   # Configuration management
├── Editor/                # ImGui-based development interface
│   ├── GameEditor.h/cpp   # Main editor implementation
│   ├── imgui/             # ImGui library
│   └── rlImGui/           # Raylib-ImGui integration
├── Game/                  # Application entry points
│   ├── main.cpp           # Editor entry point
│   ├── game.cpp           # Runtime-only entry point
│   └── DllLoader.h/cpp    # Hot-reload implementation
├── GameLogic/             # Your game code (becomes DLL)
│   ├── RootManager.cpp    # DLL entry point
│   ├── Level1.h/cpp       # Example game maps
│   ├── Level2.h/cpp       # Example game maps
│   └── FireParticle.h     # Particle system
└── Assets/                # Game resources
    └── EngineContent/     # Engine UI resources
```

### Initial Setup Steps

1. **Clone Repository**
```bash
git clone <repository-url>
cd 2D-GameEngine
```

2. **Open Developer Command Prompt**
   - Search "Developer Command Prompt for VS" in Start Menu
   - Navigate to project directory

3. **Build Project**
```bash
cmake --preset x64-debug
cmake --build out/build/x64-debug --config Debug --target main
```

4. **Run Editor**
```bash
out/build/x64-debug/main.exe
```

### Environment Verification

After setup, verify your environment:

- ✅ Editor window opens successfully
- ✅ Default maps are visible in Map Selection panel
- ✅ Player character responds to input
- ✅ Restart button forces map reload
- ✅ Particles are visible and animated

---

## Hot-Reload Development

### Understanding the Hot-Reload Cycle

The hot-reload system enables rapid iteration by reloading game logic without restarting the editor:

1. **Edit Code**: Modify files in `GameLogic/` folder
2. **Rebuild**: Execute build command for GameLogic target
3. **Auto-Reload**: Engine detects file change and reloads within ~0.5s
4. **Test**: Game state resets and new code is active

### Development Commands

**Rebuild GameLogic (while editor is running):**
```bash
cmake --build out/build/x64-debug --config Debug --target GameLogic
```

**Quick rebuild batch script:**
```bash
# build_gamelogic.bat
build_gamelogic.bat
```

**Rebuild Main Application (close editor first):**
```bash
cmake --build out/build/x64-debug --config Debug --target main
```

### Hot-Reload Best Practices

**1. State Management**
- Design for state reset: Assume your maps will be recreated frequently
- Use initialization patterns that work from a clean slate
- Store persistent data in configuration files or external systems

**2. Resource Cleanup**
- Use RAII patterns with smart pointers
- Unload textures and resources in destructors
- Avoid global static variables that persist across reloads

**3. Error Handling**
- Always check for compilation errors before testing
- Use console output for debugging during development
- Implement fallback behaviors for missing resources

**4. Iteration Workflow**
```cpp
// Example development pattern
class TestMap : public GameMap 
{
    Vector2 m_TestPos{400, 300};  // Quick test variable
    
public:
    void Update(float dt) override 
    {
        // Rapid prototyping code
        if (IsKeyPressed(KEY_T)) {
            std::cout << "Test position: " << m_TestPos.x << ", " << m_TestPos.y << std::endl;
        }
        
        // TODO: Remove debug code before final version
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            m_TestPos.x += 500.0f * dt;  // Fast movement for testing
        }
    }
};
```

### Troubleshooting Hot-Reload

**DLL Not Reloading:**
- Check that build completed successfully (no compiler errors)
- Verify GameLogic.dll timestamp changed
- Click "Restart" button to force reload
- Check console for error messages

**Compilation Errors:**
- Review compiler output for syntax errors
- Ensure all includes are correct
- Verify map registration syntax

**Runtime Crashes:**
- Check for null pointer dereferences
- Verify resource loading paths are correct
- Use debug builds for better error information

---

## Map Development Patterns

### Basic Map Structure

Every game map follows this pattern:

```cpp
#pragma once
#include "../Engine/GameMap.h"

class MyMap : public GameMap
{
private:
    // Private member variables for map state
    
public:
    MyMap();                              // Constructor
    ~MyMap() override = default;          // Destructor
    
    void Initialize() override;           // One-time setup
    void Update(float delta_time) override; // Per-frame logic
    void Draw() override;                 // Per-frame rendering
};
```

### Common Map Patterns

**1. Player Movement Map**
```cpp
class MovementMap : public GameMap
{
private:
    Vector2 m_PlayerPos{400.0f, 300.0f};
    float m_Speed = 200.0f;
    
public:
    void Update(float dt) override 
    {
        // 4-directional movement
        Vector2 movement{0, 0};
        if (IsKeyDown(KEY_W)) movement.y -= 1.0f;
        if (IsKeyDown(KEY_S)) movement.y += 1.0f;
        if (IsKeyDown(KEY_A)) movement.x -= 1.0f;
        if (IsKeyDown(KEY_D)) movement.x += 1.0f;
        
        // Apply movement with delta time
        m_PlayerPos.x += movement.x * m_Speed * dt;
        m_PlayerPos.y += movement.y * m_Speed * dt;
    }
    
    void Draw() override 
    {
        ClearBackground(RAYWHITE);
        DrawCircleV(m_PlayerPos, 25.0f, RED);
    }
};
```

**2. Physics Simulation Map**
```cpp
class PhysicsMap : public GameMap
{
private:
    Vector2 m_Position{400.0f, 100.0f};
    Vector2 m_Velocity{0.0f, 0.0f};
    const float GRAVITY = 980.0f;  // pixels/second²
    const float GROUND_Y = 500.0f;
    const float BOUNCE_FACTOR = 0.8f;
    
public:
    void Update(float dt) override 
    {
        // Apply gravity
        m_Velocity.y += GRAVITY * dt;
        
        // Update position
        m_Position.x += m_Velocity.x * dt;
        m_Position.y += m_Velocity.y * dt;
        
        // Ground collision
        if (m_Position.y >= GROUND_Y) {
            m_Position.y = GROUND_Y;
            m_Velocity.y = -m_Velocity.y * BOUNCE_FACTOR;
        }
        
        // Side bouncing
        if (m_Position.x <= 0 || m_Position.x >= 800) {
            m_Velocity.x = -m_Velocity.x * BOUNCE_FACTOR;
        }
    }
    
    void Draw() override 
    {
        ClearBackground(SKYBLUE);
        DrawRectangle(0, GROUND_Y, 800, 100, GREEN);
        DrawCircleV(m_Position, 20.0f, RED);
    }
};
```

**3. Interactive Menu Map**
```cpp
class MenuMap : public GameMap
{
private:
    int m_SelectedOption = 0;
    std::vector<std::string> m_MenuItems = {
        "Start Game", "Options", "Credits", "Exit"
    };
    
public:
    void Update(float dt) override 
    {
        // Navigate menu
        if (IsKeyPressed(KEY_UP)) {
            m_SelectedOption = (m_SelectedOption - 1 + m_MenuItems.size()) % m_MenuItems.size();
        }
        if (IsKeyPressed(KEY_DOWN)) {
            m_SelectedOption = (m_SelectedOption + 1) % m_MenuItems.size();
        }
        
        // Select option
        if (IsKeyPressed(KEY_ENTER)) {
            switch (m_SelectedOption) {
                case 0: RequestGotoMap("game_level"); break;
                case 1: RequestGotoMap("options_menu"); break;
                case 2: RequestGotoMap("credits"); break;
                case 3: /* Exit game */ break;
            }
        }
    }
    
    void Draw() override 
    {
        ClearBackground(DARKBLUE);
        DrawText("GAME MENU", 300, 100, 40, WHITE);
        
        for (size_t i = 0; i < m_MenuItems.size(); ++i) {
            Color color = (i == m_SelectedOption) ? YELLOW : WHITE;
            DrawText(m_MenuItems[i].c_str(), 350, 200 + i * 60, 30, color);
        }
        
        DrawText("Use UP/DOWN to navigate, ENTER to select", 250, 500, 20, LIGHTGRAY);
    }
};
```

### Advanced Map Features

**1. Map Transitions**
```cpp
// In your map's Update method
if (playerReachedExit) {
    RequestGotoMap("next_level", false);  // Go to next level
}

if (playerDied) {
    RequestGotoMap(GetMapName(), true);   // Reload current level
}
```

**2. Scene Bounds and Camera**
```cpp
void Initialize() override 
{
    SetSceneBounds(1920.0f, 1080.0f);  // Set logical scene size
}

void Draw() override 
{
    // Use scene bounds for camera calculations
    Vector2 sceneBounds = GetSceneBounds();
    // Implement camera following logic
}
```

**3. Resource Management**
```cpp
class ResourceMap : public GameMap
{
private:
    Texture2D m_BackgroundTexture;
    Sound m_BackgroundMusic;
    bool m_ResourcesLoaded = false;
    
public:
    void Initialize() override 
    {
        if (!m_ResourcesLoaded) {
            m_BackgroundTexture = LoadTexture("Assets/background.png");
            m_BackgroundMusic = LoadSound("Assets/music.wav");
            m_ResourcesLoaded = true;
        }
    }
    
    ~ResourceMap() override 
    {
        if (m_ResourcesLoaded) {
            UnloadTexture(m_BackgroundTexture);
            UnloadSound(m_BackgroundMusic);
        }
    }
};
```

---

## Asset Management

### Directory Structure

```
Assets/
├── Textures/           # PNG, JPG image files
├── Audio/              # WAV, OGG sound files
├── Fonts/              # TTF, OTF font files
├── Data/               # JSON, XML data files
└── EngineContent/      # Engine UI resources (don't modify)
    └── icons/          # Editor toolbar icons
```

### Loading Assets

**Textures:**
```cpp
// Load texture (path relative to executable)
Texture2D playerTexture = LoadTexture("Assets/Textures/player.png");

// Always check if loading succeeded
if (playerTexture.id == 0) {
    std::cerr << "Failed to load player texture!" << std::endl;
}

// Unload when done (typically in destructor)
UnloadTexture(playerTexture);
```

**Sounds:**
```cpp
Sound jumpSound = LoadSound("Assets/Audio/jump.wav");
Music backgroundMusic = LoadMusicStream("Assets/Audio/background.ogg");

// Play sounds
PlaySound(jumpSound);
PlayMusicStream(backgroundMusic);

// Cleanup
UnloadSound(jumpSound);
UnloadMusicStream(backgroundMusic);
```

**Fonts:**
```cpp
Font gameFont = LoadFont("Assets/Fonts/arial.ttf");
DrawTextEx(gameFont, "Hello World", {100, 100}, 32, 2, RED);
UnloadFont(gameFont);
```

### Asset Best Practices

**1. Consistent Naming**
- Use lowercase with underscores: `player_idle.png`
- Group by category: `enemy_goblin_walk.png`
- Include dimensions for UI: `button_200x50.png`

**2. Optimize for Target Platform**
- Keep texture sizes reasonable (1024x1024 max for most cases)
- Use appropriate compression (PNG for transparency, JPG for photos)
- Consider memory usage on target hardware

**3. Error Handling**
```cpp
class AssetManager 
{
private:
    std::unordered_map<std::string, Texture2D> m_Textures;
    
public:
    Texture2D& GetTexture(const std::string& path) 
    {
        auto it = m_Textures.find(path);
        if (it != m_Textures.end()) {
            return it->second;
        }
        
        // Load texture
        Texture2D texture = LoadTexture(path.c_str());
        if (texture.id == 0) {
            std::cerr << "Failed to load: " << path << std::endl;
            // Return a default/error texture
            static Texture2D errorTexture = LoadTexture("Assets/error.png");
            return errorTexture;
        }
        
        m_Textures[path] = texture;
        return m_Textures[path];
    }
};
```

---

This guide continues with more sections covering performance guidelines, debugging, and deployment. Would you like me to continue with the next parts?
