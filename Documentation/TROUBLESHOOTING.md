# 2D Game Engine - Troubleshooting Guide

**Version:** 1.0  
**Last Updated:** January 2025

This guide provides solutions to common problems encountered when developing with the 2D Game Engine.

---

## Table of Contents

1. [Build and Compilation Issues](#build-and-compilation-issues)
2. [Hot-Reload Problems](#hot-reload-problems)
3. [Runtime Errors](#runtime-errors)
4. [Performance Issues](#performance-issues)
5. [Asset Loading Problems](#asset-loading-problems)
6. [Editor Interface Issues](#editor-interface-issues)
7. [Distribution and Export Issues](#distribution-and-export-issues)

---

## Build and Compilation Issues

### LNK1168: Cannot open main.exe for writing

**Symptom**: Build fails with "error LNK1168: cannot open [file] for writing"

**Cause**: The executable is currently running and locked by Windows

**Solutions**:
```bash
# For main.exe - Close the editor first
# Then rebuild
cmake --build out/build/x64-debug --config Debug --target main

# For GameLogic.dll - This should work while editor is running
cmake --build out/build/x64-debug --config Debug --target GameLogic
```

**Prevention**: Only rebuild `main.exe` when the editor is closed

### CMake Configuration Failed

**Symptom**: CMake preset fails with configuration errors

**Common Causes and Solutions**:

1. **Missing Visual Studio Build Tools**
```bash
# Error: Could not find CMAKE_C_COMPILER
# Solution: Install Visual Studio Build Tools
# Or use Developer Command Prompt for VS
```

2. **Wrong Command Prompt**
```bash
# Error: 'cl' is not recognized as internal or external command
# Solution: Use "Developer Command Prompt for VS" from Start Menu
# This sets up the proper environment variables
```

3. **CMake Version Too Old**
```bash
# Error: CMake 3.10 or higher required
# Solution: Update CMake to latest version
cmake --version  # Check current version
```

### Raylib Build Failures

**Symptom**: Errors related to raylib compilation or linking

**Solutions**:
1. **Clear CMake Cache**
```bash
# Delete cache and rebuild
rmdir /s out\build
cmake --preset x64-debug
cmake --build out/build/x64-debug --config Debug
```

2. **Network Issues** (if downloading raylib fails)
```bash
# Check internet connection
# Try building again (downloads are cached)
# Manually download raylib if needed
```

### Missing Dependencies

**Symptom**: Linker errors about missing libraries or functions

**Common Missing Dependencies**:
- **OpenGL**: Usually included with graphics drivers
- **Windows SDK**: Install via Visual Studio Installer
- **MSVC Runtime**: Include redistributables in deployment

---

## Hot-Reload Problems

### DLL Not Reloading Automatically

**Symptom**: Code changes don't appear in the running game

**Diagnostic Steps**:
1. **Check Build Success**
```bash
# Verify build completed without errors
cmake --build out/build/x64-debug --config Debug --target GameLogic
# Look for "Build succeeded" message
```

2. **Verify File Timestamp**
```bash
# Check if GameLogic.dll was actually updated
dir out/build/x64-debug/GameLogic.dll
# Timestamp should be recent
```

3. **Check Console Output**
```
# Look for reload messages in console
[GameEditor] GameLogic.dll detected change, reloading...
[GameEditor] Successfully reloaded GameLogic.dll
```

**Solutions**:
- **Force Reload**: Click the "Restart" button in the editor
- **Check File Permissions**: Ensure GameLogic.dll is writable
- **Antivirus Software**: Temporarily disable real-time scanning

### Access Violation on Reload

**Symptom**: Crash with "Access violation" or segmentation fault during reload

**Common Causes**:
1. **Static Variables**: Static data persists across reloads and can cause issues
2. **Global Objects**: Global constructors/destructors can interfere
3. **Resource Cleanup**: Not properly releasing resources in destructors

**Solutions**:
```cpp
// Bad - Static variables can cause issues
static MapManager* s_manager = nullptr;  // DON'T DO THIS

// Good - Use proper ownership
class RootManager {
private:
    std::unique_ptr<MapManager> m_manager;
public:
    RootManager() : m_manager(std::make_unique<MapManager>()) {}
};

// Good - Proper resource cleanup
class MyMap : public GameMap {
    Texture2D m_texture;
public:
    MyMap() : m_texture(LoadTexture("sprite.png")) {}
    ~MyMap() { UnloadTexture(m_texture); }  // Critical!
};
```

### CreateGameMap Export Missing

**Symptom**: Editor shows "Failed to find CreateGameMap symbol"

**Cause**: Missing or incorrect DLL export declaration

**Solution**:
```cpp
// Ensure this exact signature in your GameLogic code
extern "C" __declspec(dllexport) GameMap* CreateGameMap()
{
    // Return your root map instance
    return new MyMapManager();
}
```

**Common Mistakes**:
- Wrong function name (case sensitive)
- Missing `extern "C"` (causes C++ name mangling)
- Missing `__declspec(dllexport)` (symbol not exported)
- Wrong return type

### Shadow File Accumulation

**Symptom**: Many GameLogic_*.dll files accumulating in build directory

**Cause**: Crash during reload can leave shadow files behind

**Solution**:
```bash
# Manual cleanup
del out/build/x64-debug/GameLogic_*.dll

# Or restart the editor (cleans up automatically)
```

**Prevention**: The editor normally cleans up shadow files, but crashes can interrupt this

---

## Runtime Errors

### Null Pointer Dereference

**Symptom**: Crash with "Access violation reading location 0x00000000"

**Common Causes**:
1. **Uninitialized Pointers**
```cpp
// Bad
GameMap* map;  // Uninitialized
map->Update();  // Crash

// Good
std::unique_ptr<GameMap> map = std::make_unique<MyMap>();
map->Update();  // Safe
```

2. **Failed Asset Loading**
```cpp
// Bad - No error checking
Texture2D texture = LoadTexture("missing.png");
DrawTexture(texture, 0, 0, WHITE);  // May crash if load failed

// Good - Check for errors
Texture2D texture = LoadTexture("sprite.png");
if (texture.id > 0) {  // Valid texture
    DrawTexture(texture, 0, 0, WHITE);
} else {
    std::cerr << "Failed to load texture!" << std::endl;
}
```

### Memory Access After Free

**Symptom**: Random crashes, especially during hot-reload

**Cause**: Accessing objects after they've been destroyed

**Solution**: Use RAII and smart pointers consistently
```cpp
class SafeMap : public GameMap {
private:
    std::unique_ptr<ParticleSystem> m_particles;
    std::vector<std::unique_ptr<GameObject>> m_objects;
    
public:
    // Automatic cleanup when map is destroyed
    ~SafeMap() = default;  // Smart pointers handle cleanup
};
```

### Infinite Recursion

**Symptom**: Stack overflow crash

**Common Cause**: Map transition loops
```cpp
// Bad - Can cause infinite recursion
class MapA : public GameMap {
    void Update(float dt) override {
        if (someCondition) {
            RequestGotoMap("MapB");
        }
    }
};

class MapB : public GameMap {
    void Update(float dt) override {
        if (someCondition) {
            RequestGotoMap("MapA");  // Potential loop
        }
    }
};

// Good - Add conditions to prevent loops
if (someCondition && !m_transitionRequested) {
    m_transitionRequested = true;
    RequestGotoMap("MapB");
}
```

---

## Performance Issues

### Low Frame Rate

**Symptom**: Game runs at less than 60 FPS

**Diagnostic Tools**:
```cpp
// Add to your map's Draw() method for debugging
DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, RED);
DrawText(TextFormat("Frame Time: %.2fms", GetFrameTime() * 1000), 10, 30, 20, RED);
```

**Common Causes and Solutions**:

1. **Too Many Particles**
```cpp
// Limit particle count
if (particles.size() > MAX_PARTICLES) {
    particles.erase(particles.begin());
}
```

2. **Expensive Operations in Update Loop**
```cpp
// Bad - File I/O in update loop
void Update(float dt) {
    SaveGame();  // DON'T DO THIS EVERY FRAME
}

// Good - Cache expensive operations
void Update(float dt) {
    m_saveTimer += dt;
    if (m_saveTimer > 5.0f) {  // Save every 5 seconds
        SaveGame();
        m_saveTimer = 0.0f;
    }
}
```

3. **Large Texture Operations**
```cpp
// Use texture atlases instead of many small textures
// Resize textures to appropriate size for use
```

### Memory Leaks

**Symptom**: Memory usage increases over time

**Detection**: Use task manager to monitor memory usage

**Common Causes**:
```cpp
// Bad - Manual memory management
GameMap* map = new GameMap();
// Forget to delete - MEMORY LEAK

// Good - Automatic memory management
std::unique_ptr<GameMap> map = std::make_unique<GameMap>();
// Automatically cleaned up
```

**Raylib Resource Leaks**:
```cpp
// Always pair Load/Unload functions
Texture2D texture = LoadTexture("sprite.png");
// ... use texture ...
UnloadTexture(texture);  // Don't forget this!

Sound sound = LoadSound("effect.wav");
// ... use sound ...
UnloadSound(sound);  // Don't forget this!
```

---

## Asset Loading Problems

### File Not Found Errors

**Symptom**: Console messages about missing files

**Diagnostic Steps**:
1. **Check File Paths**
```cpp
// Verify exact path and filename (case sensitive on some systems)
if (FileExists("Assets/Textures/player.png")) {
    texture = LoadTexture("Assets/Textures/player.png");
} else {
    std::cerr << "File does not exist!" << std::endl;
}
```

2. **Check Working Directory**
```cpp
// Debug: Print current working directory
std::cout << "Working directory: " << std::filesystem::current_path() << std::endl;
```

**Common Issues**:
- **Relative Paths**: Assets must be relative to executable location
- **Case Sensitivity**: Windows is case-insensitive, but be consistent
- **File Extensions**: Ensure correct extensions (.png, .wav, etc.)

### Texture Loading Failures

**Symptom**: Textures appear as white/black rectangles

**Causes and Solutions**:
1. **Unsupported Format**
```cpp
// Supported formats: PNG, JPG, GIF, BMP, TGA, PSD, PIC, PNM
// Convert unsupported formats to PNG or JPG
```

2. **Corrupted Files**
```cpp
// Verify file integrity
// Try opening file in image viewer
// Re-export from original source
```

3. **Large File Sizes**
```cpp
// Very large textures (>4096x4096) may fail
// Resize to reasonable dimensions for your use case
```

### Audio Problems

**Symptom**: No sound or audio playback issues

**Solutions**:
1. **Check Audio Format**
```cpp
// Supported: WAV, OGG, FLAC, MP3
// Convert unsupported formats
```

2. **Verify Audio Device**
```cpp
// Ensure system audio is working
// Check volume levels
// Try different audio files
```

3. **Initialize Audio**
```cpp
// Ensure audio is initialized (done by engine)
// For manual audio: InitAudioDevice()
```

---

## Editor Interface Issues

### ImGui Rendering Problems

**Symptom**: Editor interface appears corrupted or invisible

**Solutions**:
1. **Graphics Driver Update**
```
Update graphics drivers to latest version
Especially important for OpenGL compatibility
```

2. **Clear ImGui State**
```cpp
// Reset ImGui configuration (as last resort)
// Delete imgui.ini file to reset layout
```

### High DPI Display Issues

**Symptom**: Editor interface appears very small on 4K monitors

**Solutions**:
1. **Windows Display Scaling**
```
Right-click desktop > Display settings
Set scaling to 125% or 150%
May require application restart
```

2. **Manual Scaling**
```cpp
// In GameEditor.cpp (if scaling support is added)
ImGui::GetIO().FontGlobalScale = 1.5f;  // 150% scaling
```

### File Browser Not Working

**Symptom**: File dialogs don't appear or crash

**Cause**: TinyFileDialogs compatibility issue

**Solutions**:
1. **Run as Administrator** (if file access is restricted)
2. **Check Antivirus Settings** (may block file dialogs)
3. **Update Windows** (file dialog APIs may need updates)

---

## Distribution and Export Issues

### Missing Dependencies in Export

**Symptom**: Exported game fails to run on other computers

**Common Missing Files**:
```
app.exe              ✓ Main executable
raylib.dll           ✓ Graphics library
GameLogic.dll        ✓ Game logic
game_config.ini      ✓ Configuration
vcruntime140.dll     ⚠️ Often missing
msvcp140.dll         ⚠️ Often missing
```

**Solutions**:
1. **Include Visual C++ Redistributable**
```
Download from Microsoft:
"Microsoft Visual C++ Redistributable for Visual Studio 2019"
Include installer with your game
```

2. **Static Linking** (advanced)
```cmake
# Link runtime statically (increases exe size but reduces dependencies)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
```

### Export Process Hangs

**Symptom**: Export process starts but never completes

**Diagnostic Steps**:
1. **Check Export Logs** in editor export panel
2. **Monitor Task Manager** for hung processes
3. **Try Manual Export** using command line tools

**Solutions**:
- **Close Other Programs** to free up resources
- **Check Disk Space** (exports require temporary space)
- **Run Export from Command Line** to see detailed error messages

### Configuration File Issues

**Symptom**: Exported game doesn't respect window settings

**Solutions**:
1. **Verify game_config.ini Format**
```ini
# Must use exact key names (case sensitive)
width=1280
height=720
b_Fullscreen=false
b_Resizable=true
b_Vsync=true
target_fps=60
title=My Game
```

2. **Check File Location**
```
game_config.ini must be next to the executable
Not in a subfolder
```

---

## Getting Additional Help

### Debug Information

When reporting issues, include:
- **Engine Version**: Check README.md for version info
- **Windows Version**: Windows 10/11 build number
- **Hardware**: CPU, GPU, RAM specifications
- **Error Messages**: Exact text from console or error dialogs
- **Steps to Reproduce**: Specific steps that cause the problem

### Log Files

Enable detailed logging for troubleshooting:
```cpp
// Add to your map for debugging
std::cout << "[DEBUG] Map update called, dt=" << delta_time << std::endl;
```

### Community Resources

- Check Documentation folder for additional guides
- Review example code in GameLogic folder
- Test with minimal reproduction case

---

**Remember**: Most issues are solvable with careful attention to error messages and systematic debugging. When in doubt, start with a clean build and minimal test case.