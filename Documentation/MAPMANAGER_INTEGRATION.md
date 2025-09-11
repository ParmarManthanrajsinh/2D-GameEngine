# MapManager Integration with GameEngine

## Overview

This document describes the integration of the MapManager with the GameEngine to provide advanced map management capabilities while maintaining backward compatibility with existing GameMap implementations.

## Key Enhancements

### 1. GameEngine Integration

The GameEngine has been enhanced with dedicated MapManager support:

#### New Methods Added:
- `SetMapManager(std::unique_ptr<MapManager> map_manager)` - Sets the MapManager instance
- `MapManager* GetMapManager() const` - Gets the current MapManager instance
- `HasMapManager() const` - Checks if a MapManager is currently set

#### Enhanced Functionality:
- The GameEngine can now handle both regular GameMap instances and MapManager instances
- Automatic scene bounds forwarding to MapManager
- Proper initialization of MapManager when set

### 2. GameEditor Updates

The GameEditor has been updated to automatically detect and properly handle MapManager instances:

#### Automatic Detection:
- Uses `dynamic_cast` to detect if a loaded map is a MapManager
- Routes MapManager instances to the dedicated `SetMapManager` method
- Maintains backward compatibility with regular GameMap instances

### 3. DLL Compatibility

The MapManager now provides dual factory functions for DLL compatibility:

#### Factory Functions:
- `CreateGameMap()` - Returns a MapManager pointer (for direct MapManager usage)
- `CreateGameMapCompatible()` - Returns a GameMap pointer (for backward compatibility)

## Usage Examples

### Creating a MapManager-based Game

```cpp
// In your GameLogic DLL
extern "C" __declspec(dllexport) GameMap* CreateGameMap()
{
    // Create and configure your MapManager
    MapManager* manager = new MapManager();
    
    // Register your game maps
    manager->RegisterMap<MainMenuMap>("main_menu");
    manager->RegisterMap<Level1Map>("level_1");
    manager->RegisterMap<BossFightMap>("boss_fight");
    
    return manager;
}
```

### Using MapManager Features

```cpp
// Switch between maps
manager->GoToMap("level_1");

// Check current map
if (manager->IsCurrentMap("boss_fight")) {
    // Handle boss fight specific logic
}

// Get available maps
auto maps = manager->GetAvailableMaps();
```

## Backward Compatibility

The integration maintains full backward compatibility:

1. **Existing GameMap implementations** continue to work without changes
2. **GameEditor** automatically detects and handles both GameMap and MapManager instances
3. **DLL loading system** works with both factory function types
4. **GameEngine** can handle both map types seamlessly

## Benefits

### For Game Developers:
- Simplified map management with registration and switching APIs
- Default map fallback for better error handling
- Enhanced debugging information
- Demo maps for quick prototyping

### For Engine Developers:
- Clean separation of concerns
- Proper memory management with smart pointers
- Extensible architecture
- Robust error handling

## Integration Flow

1. **DLL Loading**: GameEditor loads GameLogic DLL
2. **Factory Detection**: GameEditor calls appropriate factory function
3. **Instance Creation**: MapManager is created and configured
4. **Type Detection**: GameEditor detects MapManager type using dynamic_cast
5. **Proper Routing**: MapManager is set using SetMapManager method
6. **Game Loop**: GameEngine automatically uses MapManager in Draw/Update calls

## Memory Management

The integration follows modern C++ best practices:

- **Smart Pointers**: Uses `std::unique_ptr` for automatic memory management
- **RAII**: Proper resource acquisition and release
- **Exception Safety**: Try-catch blocks for robust error handling
- **Clean Up**: Automatic cleanup in destructors

This integration provides a powerful yet simple way to manage game maps while maintaining the flexibility and extensibility of the 2D-GameEngine architecture.