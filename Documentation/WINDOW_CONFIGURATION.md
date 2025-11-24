# Window Configuration System

This document describes the new window configuration system that allows users to set resolution, fullscreen mode, and other window properties for exported games.

## Overview

The game engine now supports configurable window settings through:
- **Configuration file**: `game_config.ini` 
- **Export UI**: Configure settings during export
- **Runtime controls**: Alt+Enter to toggle fullscreen

## Configuration File Format

The `game_config.ini` file uses a simple key=value format:

```ini
# Game Configuration File
# Window Settings
width=1280
height=720
fullscreen=false
resizable=true
vsync=true
targetFPS=60
title=My Game
```

### Configuration Options

| Setting | Type | Default | Description |
|---------|------|---------|-------------|
| `width` | integer | 1280 | Window width in pixels |
| `height` | integer | 720 | Window height in pixels |
| `fullscreen` | boolean | false | Start in fullscreen mode |
| `resizable` | boolean | true | Allow window resizing |
| `vsync` | boolean | true | Enable vertical sync |
| `targetFPS` | integer | 60 | Target frame rate (0 = unlimited) |
| `title` | string | "My Game" | Window title |

### Value Formats
- **Integers**: Plain numbers (e.g., `1920`, `1080`)
- **Booleans**: `true`/`false` or `1`/`0`
- **Strings**: Plain text (no quotes needed)

## Export Configuration

When exporting a game from the editor, you can configure window settings:

### Resolution Settings
- **Width/Height inputs**: Custom resolution
- **Resolution presets**: Common resolutions dropdown
  - 1920x1080 (Full HD)
  - 1600x900 (HD+)
  - 1280x720 (HD)
  - 1024x768 (4:3)
  - 800x600 (SVGA)

### Window Mode Options
- **Start in Fullscreen**: Game launches in fullscreen
- **Resizable Window**: Allow users to resize the window
- **V-Sync**: Enable vertical synchronization
- **Target FPS**: Set frame rate limit (0 for unlimited)

### FPS Presets
Common frame rate options:
- 30 FPS (battery saving)
- 60 FPS (standard)
- 120 FPS (high refresh rate)
- 144 FPS (gaming monitors)
- 240 FPS (competitive gaming)
- 0 (unlimited)

### Validation
- Minimum resolution: 320x240
- Maximum resolution: 7680x4320 (8K)
- FPS range: 0-1000 (0 = unlimited)
- Invalid values are automatically corrected

## Runtime Controls

### Keyboard Shortcuts
- **Alt+Enter**: Toggle between fullscreen and windowed mode

### FPS Behavior
- **When V-Sync is enabled**: Frame rate is controlled by display refresh rate (targetFPS is ignored)
- **When V-Sync is disabled**: Game runs at specified targetFPS
- **targetFPS = 0**: Unlimited frame rate (may cause high CPU/GPU usage)
- **Performance impact**: Higher FPS uses more system resources

### Programmatic Control
Developers can control window settings programmatically:

```cpp
#include "GameConfig.h"
#include "GameEngine.h"

// Load and modify configuration
GameConfig& config = GameConfig::GetInstance();
config.LoadFromFile("game_config.ini");

// Change settings
config.GetWindowConfig().width = 1920;
config.GetWindowConfig().height = 1080;
config.GetWindowConfig().fullscreen = true;

// Save changes
config.SaveToFile("game_config.ini");

// Initialize engine with configuration
GameEngine engine;
engine.LaunchWindow(config.GetWindowConfig());

// Toggle fullscreen at runtime
engine.ToggleFullscreen();

// Set specific window mode
engine.SetWindowMode(true);  // fullscreen
engine.SetWindowMode(false); // windowed
```

## File Locations

### Development Environment
- Source: `game_config.ini` (project root)
- Distributed: `dist/game_config.ini`

### Exported Games
- Configuration: `game_config.ini` (next to game executable)

## Migration Guide

### From Hardcoded Resolution
Old code:
```cpp
engine.LaunchWindow(1280, 720, "My Game");
```

New code:
```cpp
GameConfig& config = GameConfig::GetInstance();
config.LoadFromFile("game_config.ini");
engine.LaunchWindow(config.GetWindowConfig());
```

### Export Process Updates
1. Set your desired resolution in the Export panel
2. Configure window mode options
3. Export creates `game_config.ini` with your settings
4. End users can edit the config file to change settings

## Troubleshooting

### Configuration Not Loading
- **Check file location**: Must be next to the executable
- **Check file format**: Use simple `key=value` format
- **Check permissions**: File must be readable

### Invalid Resolution
- Game will use defaults if invalid resolution is specified
- Check console output for configuration loading messages

### Fullscreen Issues
- Some displays may not support the specified resolution in fullscreen
- Alt+Enter can be used to toggle back to windowed mode
- Edit `game_config.ini` and set `fullscreen=false`

## Best Practices

### For Developers
1. **Test multiple resolutions**: Ensure your game works at different sizes
2. **Handle aspect ratios**: Consider how different ratios affect gameplay
3. **Provide fallbacks**: Always have sensible defaults

### For End Users
1. **Start windowed**: If you have display issues, set `fullscreen=false`
2. **Common resolutions**: Use standard resolutions for best compatibility
3. **Backup config**: Keep a copy of working configuration

### For Distribution
1. **Include documentation**: Explain configuration options to users
2. **Test thoroughly**: Verify exported games work on target systems
3. **Provide presets**: Consider including multiple configuration files

## Technical Details

### Implementation Files
- `Engine/GameConfig.h` - Configuration class definition
- `Engine/GameConfig.cpp` - Configuration implementation
- `Engine/GameEngine.h` - Updated engine interface
- `Engine/GameEngine.cpp` - Window management implementation
- `Game/game.cpp` - Runtime configuration loading

### Build System
- CMakeLists.txt updated for new source files
- Export scripts create configuration files
- Distribution includes configuration files

This system provides a flexible and user-friendly way to configure window settings while maintaining backward compatibility with existing projects.