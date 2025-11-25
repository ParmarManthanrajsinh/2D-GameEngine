# 2D Game Engine - Architecture Documentation

**Version:** 1.0  
**Last Updated:** January 2025

This document provides detailed technical information about the engine's architecture, design decisions, and implementation details.

---

## Table of Contents

1. [System Architecture](#system-architecture)
2. [Hot-Reload Implementation](#hot-reload-implementation)
3. [Memory Management](#memory-management)
4. [Threading Model](#threading-model)
5. [Platform Dependencies](#platform-dependencies)
6. [Performance Characteristics](#performance-characteristics)

---

## System Architecture

### Component Hierarchy

```
Main Application (main.exe)
├── GameEditor (ImGui Interface)
│   ├── Scene Rendering Window
│   ├── Map Selection Panel
│   ├── Export Configuration
│   └── File Browser
├── GameEngine (Core Runtime)
│   ├── Window Management (Raylib)
│   ├── Render Loop
│   ├── Input Handling
│   └── Map Lifecycle
└── DllLoader (Hot-Reload System)
    ├── Shadow File Management
    ├── Library Loading/Unloading
    └── Symbol Resolution

GameLogic.dll (Hot-Reloadable)
├── MapManager (Scene Management)
│   ├── Map Registry
│   ├── Transition System
│   └── Instance Management
├── Custom Game Maps
│   ├── Level1, Level2, etc.
│   ├── Game Logic Implementation
│   └── Asset References
└── Particle Systems
    ├── FireParticle Implementation
    ├── Emitter Management
    └── Rendering Pipeline

Engine Static Library
├── GameMap (Base Class)
├── GameConfig (Configuration)
├── Core Interfaces
└── Utility Functions
```

### Data Flow

**Initialization Sequence:**
1. Main application starts and initializes Raylib
2. GameEditor creates UI context and loads theme
3. GameEngine initializes with window configuration
4. DllLoader attempts to load GameLogic.dll
5. CreateGameMap() function called to instantiate root map
6. MapManager (if used) registers and loads initial map
7. Main loop begins with Update/Draw cycle

**Runtime Loop:**
```
┌─────────────────────────────────────────┐
│              Main Loop                  │
│                                         │
│  ┌─────────────┐  ┌─────────────────┐   │
│  │  ImGui UI   │  │   Game Engine   │   │
│  │   Update    │  │     Update      │   │
│  └─────────────┘  └─────────────────┘   │
│          │                │             │
│          │                ▼             │
│          │    ┌─────────────────────┐   │
│          │    │    Active Map       │   │
│          │    │   Update/Draw       │   │
│          │    └─────────────────────┘   │
│          │                │             │
│          ▼                ▼             │
│  ┌─────────────────────────────────┐   │
│  │        Render Target            │   │
│  │    (Combined Output)            │   │
│  └─────────────────────────────────┘   │
│                                         │
│  ┌─────────────────────────────────┐   │
│  │       File Watcher              │   │
│  │   (Background Thread)           │   │
│  └─────────────────────────────────┘   │
└─────────────────────────────────────────┘
```

**Hot-Reload Sequence:**
1. File watcher detects GameLogic.dll modification
2. Current map instance is safely destroyed
3. Old DLL is unloaded and shadow file deleted
4. New shadow copy is created from updated DLL
5. New DLL is loaded and CreateGameMap() is called
6. New map instance replaces the old one
7. Game loop continues with new logic

### Interface Contracts

**DLL Export Interface:**
```cpp
extern "C" __declspec(dllexport) GameMap* CreateGameMap();
```
- **Stability**: This interface must remain stable across all hot-reloads
- **Ownership**: Returns a raw pointer that the engine takes ownership of
- **Threading**: Called only from the main thread
- **Error Handling**: Must return valid GameMap* or engine will use fallback

**GameMap Virtual Interface:**
```cpp
class GameMap {
    virtual void Initialize();     // Optional: Setup resources
    virtual void Update(float);    // Required: Game logic per frame
    virtual void Draw();          // Required: Rendering per frame
    virtual ~GameMap();           // Required: Cleanup resources
};
```

---

## Hot-Reload Implementation

### Shadow Copy Mechanism

The hot-reload system uses shadow copying to work around Windows DLL file locking:

**Problem**: Windows locks loaded DLLs, preventing recompilation
**Solution**: Load a shadow copy, leaving original file writable

```cpp
// Simplified shadow copy process
std::string CreateShadowCopy(const std::string& original_path) {
    // Generate unique shadow filename
    auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    std::string shadow_path = "GameLogic_" + std::to_string(timestamp) + ".dll";
    
    // Copy file
    std::filesystem::copy_file(original_path, shadow_path);
    
    return shadow_path;
}
```

**Benefits:**
- Original file remains unlocked for rebuilding
- Multiple shadow copies can coexist temporarily
- Atomic replacement of loaded code

**Drawbacks:**
- Disk space overhead (temporary files)
- Cleanup complexity on crashes
- Platform-specific (Windows only)

### File Monitoring

The engine monitors GameLogic.dll for changes using filesystem timestamps:

```cpp
class FileWatcher {
private:
    fs::file_time_type m_LastWriteTime;
    std::string m_WatchedFile;
    float m_CheckInterval = 0.5f;  // Check every 500ms
    
public:
    bool HasChanged() {
        auto current_time = fs::last_write_time(m_WatchedFile);
        if (current_time != m_LastWriteTime) {
            m_LastWriteTime = current_time;
            return true;
        }
        return false;
    }
};
```

**Optimization Considerations:**
- Polling interval balances responsiveness vs CPU usage
- File system caching affects detection timing
- Network drives may have different characteristics

### Memory Safety During Reload

**Critical Safety Measures:**

1. **Map Destruction**: Current map is destroyed before DLL unload
2. **Reference Cleanup**: All pointers to DLL code are cleared
3. **Resource Release**: Textures/sounds loaded by map are unloaded
4. **Thread Synchronization**: Reload only occurs on main thread

```cpp
void SafeReload() {
    // 1. Destroy current map (calls destructor)
    m_GameMap.reset();
    
    // 2. Unload DLL (invalidates all code pointers)
    UnloadDll(m_DllHandle);
    
    // 3. Clean up shadow file
    fs::remove(m_DllHandle.shadow_path);
    
    // 4. Load new version
    m_DllHandle = LoadDll("GameLogic.dll");
    
    // 5. Create new map instance
    auto createFunc = GetDllSymbol(m_DllHandle, "CreateGameMap");
    m_GameMap.reset(createFunc());
}
```

### Error Recovery

**Reload Failure Handling:**
- If new DLL fails to load, engine continues with last working version
- If CreateGameMap fails, engine provides minimal default map
- Console logging helps developers diagnose issues
- Manual restart button provides fallback mechanism

---

## Memory Management

### Resource Ownership Model

**Engine Resources** (Persistent across reloads):
- Window and graphics context
- ImGui state and themes
- Icon textures for editor UI
- Configuration data

**DLL Resources** (Recreated on reload):
- Game map instances
- Map-specific textures and sounds
- Particle system state
- Game logic variables

**Shared Resources** (Reference counted):
- Raylib graphics context (shared DLL)
- Common utility functions
- Configuration files

### Smart Pointer Usage

The engine uses modern C++ memory management:

```cpp
class GameEngine {
private:
    std::unique_ptr<GameMap> m_GameMap;        // Exclusive ownership
    std::unique_ptr<MapManager> m_MapManager;  // Exclusive ownership
    
public:
    void SetMap(std::unique_ptr<GameMap> map) {
        m_GameMap = std::move(map);  // Transfer ownership
    }
};
```

**Benefits:**
- Automatic cleanup on scope exit
- Exception safety
- Clear ownership semantics
- Prevents double-deletion bugs

### Memory Layout Considerations

**Static Variables**: Avoided in hot-reloadable code due to persistence issues
**Global State**: Minimized and carefully managed
**Stack vs Heap**: Large objects allocated on heap for better cache behavior

---

## Threading Model

### Single-Threaded Design

The engine uses a single-threaded model for simplicity and hot-reload safety:

**Main Thread Responsibilities:**
- Raylib rendering and input handling
- ImGui UI updates
- Game logic execution
- File monitoring and hot-reload triggers
- Asset loading and management

**Benefits:**
- No synchronization complexity
- Simpler debugging and development
- Hot-reload safety (no race conditions)
- Predictable execution order

**Limitations:**
- CPU-intensive tasks block rendering
- No parallel processing of game logic
- File I/O can cause frame drops

### Asynchronous Operations

Limited async operations are used where necessary:

**Export Process**: Runs on background thread
```cpp
std::thread m_ExportThread;
std::atomic<bool> m_bIsExporting{false};
std::mutex m_ExportLogMutex;
```

**File Operations**: Some file copying uses async I/O
**Build Process**: External compiler runs in separate process

### Thread Safety Considerations

When implementing threaded features:

1. **Atomic Flags**: Use `std::atomic<bool>` for simple state
2. **Mutex Protection**: Protect shared data structures
3. **Message Passing**: Prefer messages over shared memory
4. **Main Thread Callbacks**: Schedule updates on main thread

---

## Platform Dependencies

### Windows-Specific Features

**DLL Loading**: Uses Windows LoadLibrary/GetProcAddress
```cpp
HMODULE handle = LoadLibraryA(shadow_path.c_str());
FARPROC symbol = GetProcAddress(handle, "CreateGameMap");
```

**File System**: Uses Windows file paths and operations
**Build System**: Assumes MSVC compiler and tools
**Visual Studio**: Developer Command Prompt required

### Cross-Platform Abstraction Opportunities

**DLL Loading**: Could be abstracted for Unix shared libraries
**File Monitoring**: Could use inotify (Linux) or kqueue (macOS)
**Build System**: CMake already provides cross-platform support
**Path Handling**: std::filesystem provides portable paths

### Porting Considerations

To support other platforms:

1. **Abstract DLL Loading**: Create platform-specific implementations
2. **File Monitoring**: Implement native file watchers
3. **Build Scripts**: Create platform-specific build helpers
4. **Testing**: Verify Raylib compatibility on target platforms

---

## Performance Characteristics

### Rendering Performance

**Target**: 60 FPS at 1080p resolution
**Bottlenecks**: Particle systems, texture loading, large map rendering
**Optimization**: Batch rendering, texture atlases, culling

### Hot-Reload Performance

**Reload Time**: ~0.5 seconds typical
**Factors Affecting Speed**:
- DLL size (larger = slower loading)
- Disk I/O speed (SSD recommended)
- Antivirus scanning (can add delays)
- Map initialization complexity

### Memory Usage

**Typical Footprint**:
- Engine core: ~50MB
- ImGui interface: ~10MB
- Game maps: Variable (depends on assets)
- Particle systems: ~1-5MB per system

**Memory Growth**: Minimal during normal operation
**Leak Prevention**: RAII patterns and automatic cleanup

### CPU Usage

**Idle**: ~1-2% (60 FPS empty scene)
**Active Game**: ~10-30% (depends on game complexity)
**Peak Usage**: During compilation and hot-reload

### Optimization Guidelines

1. **Profile First**: Use tools to identify actual bottlenecks
2. **Batch Operations**: Group similar rendering calls
3. **Limit Particles**: Keep particle counts reasonable
4. **Asset Optimization**: Use appropriate texture sizes and formats
5. **Update Frequency**: Consider variable time steps for non-critical systems

---

**Next Steps**: This architecture documentation should be updated as the engine evolves. Consider adding UML diagrams and performance benchmarks for future versions.