# 2D Game Engine - Distribution Package

This package contains a complete Raylib-based game engine/editor with hot-reloading capabilities. The engine maintains Raylib's coding style while providing an editor environment for rapid game development.

## What's Included

- **app.exe** - The main game engine/editor executable
- **raylib.dll** - Raylib graphics library (shared)
- **GameLogic.dll** - Hot-reloadable game logic library
- **CMakeLists.txt** - Build configuration for GameLogic development
- **GameLogic/** - Source code folder for your game logic
- **Engine/** - Engine source and header files (for GameLogic compilation)
- **Assets/** - Game assets (textures, shaders, etc.)

## Quick Start

1. **Run the Engine/Editor**
   ```
   ./app.exe
   ```

2. **Develop Your Game Logic**
   - Edit files in the `GameLogic/` folder
   - The engine automatically detects changes and hot-reloads your code

3. **Build Your Game Logic**
   ```powershell
   # Create build directory
   mkdir build
   cd build
   
   # Configure CMake
   cmake ..
   
   # Build GameLogic.dll
   cmake --build . --target GameLogic
   ```

## How It Works

### Hot-Reloading System
- The engine loads `GameLogic.dll` at runtime
- When you rebuild the DLL, the engine detects the change
- Your game code is automatically reloaded (within ~0.5 seconds)
- Game state resets during reload (this is intentional)

### GameLogic Structure
Your game logic should follow this pattern:

```cpp
// In GameLogic/YourMap.h
#include "../Engine/GameMap.h"

class YourMap : public GameMap {
    // Your game variables
    Vector2 playerPosition{0.0f, 0.0f};
    
public:
    void Update(float deltaTime) override {
        // Game logic here - pure Raylib style
        if (IsKeyDown(KEY_RIGHT)) playerPosition.x += 100.0f * deltaTime;
    }
    
    void Draw() override {
        // Rendering here - pure Raylib style
        DrawCircleV(playerPosition, 20.0f, RED);
    }
};
```

```cpp
// In GameLogic/RootManager.cpp
#include "YourMap.h"

extern "C" __declspec(dllexport) GameMap* CreateGameMap() {
    MapManager* manager = new MapManager();
    manager->RegisterMap<YourMap>("your_map");
    // Load the first registered map
    auto maps = manager->GetAvailableMaps();
    if (!maps.empty()) {
        manager->b_GotoMap(maps[0]);
    }
    return manager;
}
```

## Development Workflow

### 1. Standard Development
- Run `app.exe` to start the editor
- Modify code in `GameLogic/`
- Build with CMake when ready to test
- Engine auto-reloads your changes

### 2. Using Visual Studio
- Open the folder in Visual Studio
- Use "Open Folder" and select this directory
- Visual Studio will recognize the CMake project
- Build GameLogic target for hot-reloading

### 3. Command Line Development
```powershell
# Quick rebuild during development
cmake --build build --target GameLogic

# The engine will detect and reload automatically
```

## Engine Features

### Editor Interface
- **Scene Window**: Live preview of your game
- **File Explorer**: Browse and manage assets
- **Toolbar**: Play/Pause/Restart controls
- **Map Selection**: Switch between different game maps

### Hot-Reload Benefits
- **Fast Iteration**: No need to restart the engine
- **Live Debugging**: See changes immediately
- **Asset Management**: Integrated file browser
- **Multiple Maps**: Easy switching between game scenes

## File Structure Explained

```
Distribution/
├── app.exe              # Main engine executable
├── raylib.dll           # Graphics library
├── GameLogic.dll        # Your game code (hot-reloadable)
├── CMakeLists.txt       # Build configuration
├── GameLogic/           # Your source code
│   ├── RootManager.cpp  # DLL entry point
│   ├── DefaultMap.cpp   # Example map
│   ├── TestMap.cpp      # Example map
│   └── *.h/.cpp         # Your game files
├── Engine/              # Engine source files
│   ├── GameMap.h        # Base class for your maps
│   ├── GameMap.cpp      # GameMap implementation
│   ├── GameEngine.h     # Core engine interface
│   ├── GameEngine.cpp   # GameEngine implementation
│   ├── MapManager.h     # Map management system
│   └── MapManager.cpp   # MapManager implementation
└── Assets/              # Game assets
    ├── Shaders/         # GLSL shaders
    └── icons/           # UI icons
```

## Requirements

- **Windows 10/11**
- **Visual Studio Build Tools** or **Visual Studio** (for MSVC compiler)
- **CMake 3.10+**
- **Developer Command Prompt** (for cl.exe access)

## Building GameLogic

### Using Developer Command Prompt
```cmd
# Navigate to distribution folder
cd path/to/distribution

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
cmake --build . --target GameLogic
```

### Using Visual Studio
1. Open the distribution folder in Visual Studio
2. Select "GameLogic" as the startup target
3. Build the project (Ctrl+Shift+B)

## Troubleshooting

### DLL Not Reloading
- Ensure the build completed successfully
- Check that `GameLogic.dll` timestamp changed
- Use the Restart button in the editor toolbar

### Build Errors
- Ensure you're using a Developer Command Prompt
- Verify CMake and Visual Studio Build Tools are installed
- Check that `cl.exe` is in your PATH

### Missing raylib.dll
- The CMake configuration should handle this automatically
- If missing, ensure raylib builds correctly

## Example Maps

The distribution includes example maps to get you started:

- **DefaultMap**: Shows basic instructions and engine info
- **TestMap**: Demonstrates basic game objects and movement

Study these examples to understand the coding patterns and engine integration.

## Pure Raylib Style

This engine maintains Raylib's simplicity:
- Direct Raylib function calls
- Simple game loops (Update/Draw)
- No complex abstractions
- Familiar Raylib patterns
- Easy asset loading

You write normal Raylib code - the engine just provides the editor environment and hot-reloading!