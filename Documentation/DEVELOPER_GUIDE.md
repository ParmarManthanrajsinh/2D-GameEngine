# RayWaves Engine - Developer Guide

Development patterns and best practices for RayWaves.

## Development Workflow

1. **Run the editor**: `out/build/x64-debug/main.exe`
2. **Edit game code**: Modify files in `GameLogic/`
3. **Rebuild**: `cmake --build out/build/x64-debug --config Debug --target GameLogic`
4. **See changes instantly**: Editor auto-reloads your code

## Project Structure

```
RayWaves/
├── Engine/          # Core engine classes (GameMap, MapManager, GameConfig)
├── Editor/          # ImGui editor and UI
├── Game/            # Program entry points and DLL loader
├── GameLogic/       # Your game code (builds as GameLogic.dll)
├── Assets/          # Game assets and resources
└── Distribution/    # Distribution and packaging
```

## Hot-Reload System

- **GameLogic.dll** contains your game code
- Engine creates shadow copy to avoid Windows file locking
- File watcher detects changes and triggers automatic reload
- Game state resets on reload (intentional for testing)

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