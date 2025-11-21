# 2D Game Engine (Windows)

A simple Raylib-based game engine with **hot-reloading** support. Edit your game code and see changes instantly without restarting!

## ✨ Features

- 🔥 **Hot-Reload** - Edit code and see changes in ~0.5 seconds
- 🎮 **Pure Raylib** - Write normal Raylib code, no complex abstractions
- 🖼️ **Visual Editor** - ImGui-based editor with file browser
- 🗺️ **Map System** - Easy scene/level switching with MapManager
- 📦 **Easy Distribution** - Share your engine with other developers

## 🚀 Quick Start

### Requirements
- Windows 10/11
- Visual Studio Build Tools (MSVC)
- CMake 3.10+
- Developer Command Prompt for VS

### Build and Run

1. **Open Developer Command Prompt for VS**

2. **Build the project:**
   ```powershell
   cmake --preset x64-debug
   cmake --build out/build/x64-debug --config Debug --target main
   ```

3. **Run the editor:**
   ```powershell
   out/build/x64-debug/main.exe
   ```

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
2D-GameEngine/
├── Engine/          # Core engine (GameEngine, GameMap, MapManager)
├── Editor/          # ImGui editor and UI
├── Game/            # Program entry and DLL loader
├── GameLogic/       # Your game code (built as GameLogic.dll)
│   ├── RootManager.cpp    # DLL entry point
│   ├── DefaultMap.cpp     # Example map
│   └── TestMap.cpp        # Example map
└── Assets/          # Images, fonts, shaders, etc.
```

## 🗺️ Map System

The engine includes a **MapManager** for easy scene/level management:

### Creating Maps

```cpp
// YourMap.h
#include "../Engine/GameMap.h"

class YourMap : public GameMap {
    Vector2 playerPos{0.0f, 0.0f};
    
public:
    void Update(float deltaTime) override {
        if (IsKeyDown(KEY_RIGHT)) playerPos.x += 100.0f * deltaTime;
    }
    
    void Draw() override {
        DrawCircleV(playerPos, 20.0f, RED);
    }
};
```

### Registering Maps

```cpp
// GameLogic/RootManager.cpp
#include "../Engine/MapManager.h"
#include "YourMap.h"

extern "C" __declspec(dllexport) GameMap* CreateGameMap() {
    MapManager* manager = new MapManager();
    
    // Register your maps
    manager->RegisterMap<YourMap>("your_map");
    manager->RegisterMap<Level1>("level_1");
    
    // Load the first map
    auto maps = manager->GetAvailableMaps();
    if (!maps.empty()) {
        manager->GoToMap(maps[0]);
    }
    
    return manager;
}
```

### Switching Maps

```cpp
// In your game code
manager->GoToMap("level_1");

// Check current map
if (manager->IsCurrentMap("boss_fight")) {
    // Boss fight logic
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

Want to share your engine with others? See [Distribution Guide](Documentation/DISTRIBUTION_GUIDE.md) for detailed instructions.

**Quick distribution:**
```cmd
create_distribution.bat
```

This creates a complete package in the `dist/` folder with everything needed for others to develop games.

## 📚 Documentation

- **[Distribution Guide](Documentation/DISTRIBUTION_GUIDE.md)** - How to create distribution packages
- **[Distribution README](Documentation/README_DISTRIBUTION.md)** - User guide for distributed packages

## 🎮 Development Workflow

1. **Run the editor** - `main.exe`
2. **Edit your game code** - Modify files in `GameLogic/`
3. **Rebuild GameLogic** - `cmake --build out/build/x64-debug --target GameLogic`
4. **See changes instantly** - Editor auto-reloads your code

## 💡 Tips

- Use the **File Explorer** in the editor to browse assets
- The **Restart** button forces a reload if auto-reload doesn't trigger
- Game state resets on reload (this is intentional)
- Write pure Raylib code - no complex abstractions needed

---

**Happy Game Development! 🎮**
