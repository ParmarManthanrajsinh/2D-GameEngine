# RayWaves Game Engine

*Where code changes flow like waves* 🌊

A simple Raylib-based game engine with **hot-reloading** support. Edit your game code and see changes flow through instantly without restarting!

## ✨ Features

- 🔥 **Hot-Reload** - Edit code and see changes in ~0.5 seconds
- 🎮 **Pure Raylib** - Write normal Raylib code, no complex abstractions
- 🖼️ **Visual Editor** - ImGui-based editor with file browser
- 🗺️ **Map System** - Easy scene/level switching with MapManager
- 📦 **Distribution System** - Package your engine for others to use

## 📋 Requirements

**Windows Only**
- Windows 10/11
- **Visual Studio** (Community, Professional, or Enterprise) with C++ development tools
- **CMake 3.10+** 
- **Raylib** (automatically downloaded by CMake)

## 🚀 Quick Start

### Build and Run

1. **Open Developer Command Prompt for VS** (or use Visual Studio's terminal)

2. **Build the project:**
   ```powershell
   cmake --preset x64-debug
   cmake --build out/build/x64-debug --config Debug --target main
   ```

3. **Run the editor:**
   ```powershell
   out/build/x64-debug/main.exe
   ```

> ⚠️ **Important Note for Visual Studio Users:**  
> When running the source code directly in Visual Studio, the **export** and **hot reload** features will **not work as intended**. These features are designed to work properly in the **distribution build** only. For full functionality, use the command-line build process above or create a distribution package.

## 🎯 How It Works

### Hot-Reloading System
Your game code lives in `GameLogic.dll`, which the editor loads at runtime. When you rebuild the DLL, the editor automatically detects the change and reloads it.

**Why this works:**
- Windows locks DLLs when loaded
- Our loader creates a shadow copy of the DLL
- The original file stays unlocked for rebuilding
- Editor watches for file changes and reloads automatically

### Rebuild GameLogic While Running

```powershell
cmake --build out/build/x64-debug --config Debug --target GameLogic
```

The editor will auto-reload within ~0.5 seconds, or click the **Restart** button.

> **Note:** To rebuild `main.exe`, close the app first (otherwise you'll get LNK1168 error).

## 📁 Project Structure

```
RayWaves/
├── Distribution/    # Distribution and packaging files
│   ├── build_gamelogic.bat    # Quick build script for GameLogic.dll
│   ├── create_distribution.bat # Main distribution creation script
│   ├── dist_CMakeLists.txt    # CMake config for distributed environment
│   ├── distribute.ps1         # PowerShell packaging script
│   └── config.ini             # Default game configuration
├── Engine/          # Core engine (GameEngine, GameMap, MapManager, GameConfig)
├── Editor/          # ImGui editor and UI components
│   ├── imgui/               # ImGui library
│   ├── rlImGui/            # Raylib-ImGui integration
│   └── tinyfiledialogs/    # File dialog library
├── Game/            # Program entry points and DLL loader
│   ├── main.cpp            # Editor entry point
│   ├── game.cpp            # Runtime-only entry point
│   └── DllLoader.cpp       # Hot-reload DLL management
├── GameLogic/       # Your game code (built as GameLogic.dll)
│   ├── RootManager.cpp     # DLL entry point and map registration
│   ├── Level1.cpp/h        # Example game level
│   ├── Level2.cpp/h        # Another example level
│   └── FireParticle.h      # Example particle system
├── Assets/          # Game assets
│   └── EngineContent/      # Built-in engine assets (icons, etc.)
└── Documentation/   # Complete documentation and guides
```

## 🗺️ Map System

The engine includes a **MapManager** for easy scene/level management:

### Creating Maps

```cpp
// YourMap.h
#pragma once
#include "../Engine/GameMap.h"
#include <raylib.h>

class YourMap : public GameMap {
private:
    Vector2 m_PlayerPos;
    const float MOVE_SPEED = 300.0f;
    
public:
    YourMap();
    ~YourMap() override = default;

    void Initialize() override;
    void Update(float delta_time) override;
    void Draw() override;
};

// YourMap.cpp
#include "YourMap.h"

YourMap::YourMap() : m_PlayerPos{400.0f, 300.0f} {}

void YourMap::Initialize() {
    // Initialize your map here
}

void YourMap::Update(float delta_time) {
    if (IsKeyDown(KEY_RIGHT)) m_PlayerPos.x += MOVE_SPEED * delta_time;
    if (IsKeyDown(KEY_LEFT)) m_PlayerPos.x -= MOVE_SPEED * delta_time;
    if (IsKeyDown(KEY_UP)) m_PlayerPos.y -= MOVE_SPEED * delta_time;
    if (IsKeyDown(KEY_DOWN)) m_PlayerPos.y += MOVE_SPEED * delta_time;
}

void YourMap::Draw() {
    DrawCircleV(m_PlayerPos, 25.0f, RED);
}
```

### Registering Maps

```cpp
// GameLogic/RootManager.cpp
#include "../Engine/MapManager.h"
#include "Level1.h"
#include "Level2.h"
#include "YourMap.h" // Your custom map

// Global static instance to ensure consistency across editor and runtime
static MapManager* s_GameMapManager = nullptr;

extern "C" __declspec(dllexport) GameMap* CreateGameMap() {
    // If we already have a manager, reuse it to maintain map registrations
    if (s_GameMapManager == nullptr) {
        s_GameMapManager = new MapManager();

        // Register your game maps - this happens only once
        s_GameMapManager->RegisterMap<Level1>("Level1");
        s_GameMapManager->RegisterMap<Level2>("Level2");
        s_GameMapManager->RegisterMap<YourMap>("YourMap"); // Add your custom maps
    }

    // Automatically load the first registered map
    auto available_maps = s_GameMapManager->GetAvailableMaps();
    if (!available_maps.empty()) {
        s_GameMapManager->b_GotoMap(available_maps.at(0));
    }

    return s_GameMapManager;
}
```

### Switching Maps

```cpp
// From within a map (e.g., in your Update() method)
void YourMap::Update(float delta_time) {
    // Switch to another map when player reaches finish zone
    if (CheckCollisionRecs(playerRect, finishZone)) {
        RequestGotoMap("Level2");  // Safe transition via callback
    }
    
    // Reload current map (e.g., when player dies)
    if (IsKeyPressed(KEY_R)) {
        RequestGotoMap(GetMapName(), true);  // force_reload = true
    }
}

// From the MapManager directly (e.g., in editor or external code)
manager->b_GotoMap("Level1");

// Check current map
if (manager->b_IsCurrentMap("Level2")) {
    // Level 2 specific logic
}

// Get current map ID
std::string currentMap = manager->GetCurrentMapId();

// Get all available maps
auto availableMaps = manager->GetAvailableMaps();
for (const auto& mapName : availableMaps) {
    std::cout << "Available map: " << mapName << std::endl;
}
```

## 🛠️ Troubleshooting

### LNK1168: cannot open main.exe for writing
**Solution:** Close the app before rebuilding `main.exe`. Rebuilding `GameLogic.dll` is safe while running.

### DLL not reloading
**Solution:** 
- Check that the build completed successfully
- Verify `GameLogic.dll` timestamp changed
- Click the **Restart** button in the editor

### Compiler not found
**Solution:** Use **Developer Command Prompt for VS** so `cl.exe` is on PATH.

### Access violation on exit
**Solution:** This has been fixed. The editor now properly destroys the map before unloading the DLL.

## 📦 Distribution

Create a complete package that others can use to develop games with your engine.

**Quick distribution:**
```cmd
Distribution\create_distribution.bat
```

This creates a `dist/` folder containing:
- `app.exe` - Combined editor and game runtime
- `GameLogic.dll` - Hot-reloadable game logic
- Complete development environment with headers and build scripts
- All necessary assets and documentation

See [Distribution Guide](Documentation/DISTRIBUTION_GUIDE.md) for detailed instructions.

## 📚 Documentation

### Core Documentation
- **[API Reference](Documentation/API_REFERENCE.md)** - Complete API documentation and examples
- **[Developer Guide](Documentation/DEVELOPER_GUIDE.md)** - Architecture, patterns, and best practices
- **[Architecture](Documentation/ARCHITECTURE.md)** - Technical architecture and design decisions
- **[Troubleshooting](Documentation/TROUBLESHOOTING.md)** - Solutions to common problems

### Specialized Guides
- **[Particle System Guide](Documentation/PARTICLE_SYSTEM_GUIDE.md)** - Comprehensive particle effects documentation
- **[Window Configuration](Documentation/WINDOW_CONFIGURATION.md)** - Window and display settings
- **[Naming Convention](Documentation/NAMING_CONVENTION.md)** - Code style guidelines

### Distribution
- **[Distribution Guide](Documentation/DISTRIBUTION_GUIDE.md)** - How to create distribution packages
- **[Distribution README](Documentation/README_DISTRIBUTION.md)** - User guide for distributed packages

## 🎮 Development Workflow

1. **Run the editor** - `out/build/x64-debug/main.exe`
2. **Edit your game code** - Modify files in `GameLogic/`
3. **Rebuild GameLogic** - `cmake --build out/build/x64-debug --config Debug --target GameLogic`
4. **See changes instantly** - Editor auto-reloads your code

## 🎯 Two Build Targets

- **`main.exe`** - Editor with hot-reload and development tools
- **`game.exe`** - Runtime-only executable for final games (no editor UI)

## 💡 Tips

- Use the **File Explorer** in the editor to browse assets
- The **Restart** button forces a reload if auto-reload doesn't trigger
- Game state resets on reload (this is intentional for consistent testing)
- Write pure Raylib code - no complex abstractions needed
- For release builds, use `--config Release` instead of `--config Debug`

---

**Happy Game Development! 🎮**
