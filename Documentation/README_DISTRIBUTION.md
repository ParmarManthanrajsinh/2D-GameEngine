# 2D Game Engine - Distribution Package

Welcome! This package contains a complete Raylib-based game engine with **hot-reloading** capabilities. Write Raylib code and see changes instantly!

## 📦 What's Included

- **app.exe** - The game engine/editor
- **raylib.dll** - Graphics library
- **GameLogic.dll** - Your hot-reloadable game code
- **CMakeLists.txt** - Build configuration
- **build_gamelogic.bat** - Quick build script
- **GameLogic/** - Source code folder
- **Engine/** - Engine headers
- **Assets/** - Game assets

## 🚀 Quick Start

### 1. Run the Engine

```
app.exe
```

### 2. Edit Your Game Code

Open and edit files in the `GameLogic/` folder using your favorite code editor.

### 3. Build and See Changes

**Option A: Using the batch script (easiest)**
```cmd
build_gamelogic.bat
```

**Option B: Using CMake**
```cmd
mkdir build
cd build
cmake ..
cmake --build . --target GameLogic
```

The engine will automatically reload your changes in ~0.5 seconds!

## 🎮 How to Create Your Game

### Step 1: Create a Map Class

```cpp
// GameLogic/MyMap.h
#include "../Engine/GameMap.h"

class MyMap : public GameMap {
    Vector2 playerPos{0.0f, 0.0f};
    
public:
    void Update(float deltaTime) override {
        // Game logic - pure Raylib style!
        if (IsKeyDown(KEY_RIGHT)) playerPos.x += 100.0f * deltaTime;
        if (IsKeyDown(KEY_LEFT)) playerPos.x -= 100.0f * deltaTime;
    }
    
    void Draw() override {
        // Rendering - pure Raylib style!
        DrawCircleV(playerPos, 20.0f, RED);
        DrawText("Use arrow keys to move", 10, 10, 20, WHITE);
    }
};
```

### Step 2: Register Your Map

```cpp
// GameLogic/RootManager.cpp
#include "../Engine/MapManager.h"
#include "MyMap.h"

extern "C" __declspec(dllexport) GameMap* CreateGameMap() {
    MapManager* manager = new MapManager();
    
    // Register your maps
    manager->RegisterMap<MyMap>("my_map");
    
    // Load the first map
    auto maps = manager->GetAvailableMaps();
    if (!maps.empty()) {
        manager->GoToMap(maps[0]);
    }
    
    return manager;
}
```

### Step 3: Build and Play!

```cmd
build_gamelogic.bat
```

Your game will appear in the editor window!

## 🗺️ Managing Multiple Maps

```cpp
// Register multiple maps
manager->RegisterMap<MainMenu>("main_menu");
manager->RegisterMap<Level1>("level_1");
manager->RegisterMap<BossLevel>("boss");

// Switch between maps
manager->GoToMap("level_1");

// Check current map
if (manager->IsCurrentMap("boss")) {
    // Boss-specific logic
}
```

## 📁 File Structure

```
Distribution/
├── app.exe              # Engine executable
├── raylib.dll           # Graphics library
├── GameLogic.dll        # Your game (hot-reloadable)
├── CMakeLists.txt       # Build config
├── build_gamelogic.bat  # Quick build script
├── GameLogic/           # Your source code
│   ├── RootManager.cpp  # Entry point
│   ├── DefaultMap.cpp   # Example map
│   └── TestMap.cpp      # Example map
├── Engine/              # Engine headers
│   ├── GameMap.h        # Base class for maps
│   ├── GameEngine.h     # Core engine
│   └── MapManager.h     # Map management
└── Assets/              # Game assets
    ├── Shaders/
    └── icons/
```

## 🛠️ Requirements

- **Windows 10/11**
- **Visual Studio Build Tools** or **Visual Studio**
- **CMake 3.10+**
- **Developer Command Prompt for VS** (for building)

## 🔧 Building GameLogic

### Using Developer Command Prompt (Recommended)

```cmd
# Navigate to distribution folder
cd path\to\distribution

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build . --target GameLogic
```

### Using Visual Studio

1. Open the distribution folder in Visual Studio
2. Select "GameLogic" as the startup target
3. Build the project (Ctrl+Shift+B)

## 🐛 Troubleshooting

### DLL Not Reloading
- Ensure the build completed successfully
- Check that `GameLogic.dll` timestamp changed
- Click the **Restart** button in the editor toolbar

### Build Errors
- Use **Developer Command Prompt for VS**
- Verify CMake and Visual Studio Build Tools are installed
- Check that `cl.exe` is in your PATH

### Missing raylib.dll
- Ensure `raylib.dll` is in the same folder as `app.exe`
- The CMake configuration should handle this automatically

## 🎯 Editor Features

- **Scene Window** - Live preview of your game
- **File Explorer** - Browse and manage assets
- **Toolbar** - Play/Pause/Restart controls
- **Map Selection** - Switch between different maps

## 📚 Example Maps

The distribution includes example maps:

- **DefaultMap** - Shows basic instructions and engine info
- **TestMap** - Demonstrates game objects and movement

Study these examples to learn the coding patterns!

## 💡 Tips

- Write **pure Raylib code** - no complex abstractions
- Use the **Update()** method for game logic
- Use the **Draw()** method for rendering
- Game state resets on reload (this is intentional)
- Hot-reload happens automatically within ~0.5 seconds

## 🎮 Pure Raylib Style

This engine maintains Raylib's simplicity:
- ✅ Direct Raylib function calls
- ✅ Simple game loops (Update/Draw)
- ✅ No complex abstractions
- ✅ Familiar Raylib patterns
- ✅ Easy asset loading

You write normal Raylib code - the engine just provides the editor and hot-reloading!

---

**Happy Game Development! 🎮**

For questions or issues, check the main project repository.