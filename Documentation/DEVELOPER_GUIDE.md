# RayWaves Engine - Developer Guide

*Where code changes flow like waves* 🌊

Development patterns and best practices for the RayWaves Game Engine.

## Development Workflow

### Source Development
1. **Run the editor**: `out/build/x64-debug/main.exe`
2. **Edit game code**: Modify files in `GameLogic/`
3. **Rebuild**: `cmake --build out/build/x64-debug --config Debug --target GameLogic`
4. **See changes instantly**: Editor auto-reloads your code

### Distribution Development (End Users)
1. **Open VS Code** in engine folder
2. **Open terminal** (`Ctrl + backtick`)
3. **Launch engine**: `./app.exe`
4. **Edit GameLogic files** and save
5. **Run build script**: `./build_gamelogic.bat`
6. **See changes flow** automatically!

## Project Structure

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

## Hot-Reload System

**How it works:**
- **GameLogic.dll** contains your game code
- Windows locks DLLs when loaded, so our loader creates a shadow copy
- The original file stays unlocked for rebuilding
- Editor watches for file changes and triggers automatic reload
- Game state resets on reload (intentional for consistent testing)

**Why this works:**
- Engine automatically detects DLL changes within ~0.5 seconds
- No need to restart the editor or lose your current state
- Write pure Raylib code - no complex abstractions needed
- Perfect for rapid iteration and testing

## Map Development

### Basic Pattern
```cpp
class YourMap : public GameMap {
private:
    Vector2 m_PlayerPos{400.0f, 300.0f};
    
public:
    YourMap() : GameMap("YourMap") {}
    
    void Update(float delta_time) override {
        // Game logic here
    }
    
    void Draw() override {
        // Rendering here
    }
};
```

### Common Patterns

**Player Input:**
```cpp
void Update(float delta_time) override {
    if (IsKeyDown(KEY_RIGHT)) m_PlayerPos.x += SPEED * delta_time;
    if (IsKeyPressed(KEY_SPACE)) Jump();
}
```

**Collision Detection:**
```cpp
Rectangle playerRect = {m_PlayerPos.x, m_PlayerPos.y, 32, 32};
if (CheckCollisionRecs(playerRect, wallRect)) {
    // Handle collision
}
```

**Level Transitions:**
```cpp
if (CheckCollisionRecs(playerRect, exitRect)) {
    RequestGotoMap("NextLevel");
}
```

## Performance Tips

1. **Use delta_time** for frame-rate independent movement
2. **Limit particles** - keep counts reasonable
3. **Batch draw calls** - group similar rendering
4. **Clean up resources** when changing maps

## Best Practices

### Code Organization
- Keep game logic in `Update()`
- Keep rendering in `Draw()`
- Use descriptive map names
- Handle edge cases (player falling off screen, etc.)

### Hot-Reload Friendly Code
- Initialize variables in constructor or `Initialize()`
- Don't rely on global state between reloads
- Test frequently with restart button

### Distribution
- Test exported games on clean systems
- Include all required assets
- Provide reasonable default settings

## Common Issues

**Hot-reload not working:**
- Check build completed successfully
- Verify GameLogic.dll timestamp changed
- Click Restart button in editor

**LNK1168 error:**
- Close editor before rebuilding main.exe
- GameLogic.dll rebuilds are safe while running

**Export issues:**
- Ensure config.ini is next to executable
- Check all assets are included
- Test on system without dev tools

---

See `API_REFERENCE.md` for class details and `TROUBLESHOOTING.md` for specific problems.