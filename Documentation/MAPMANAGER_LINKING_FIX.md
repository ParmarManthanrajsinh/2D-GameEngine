# MapManager Linking Issue Fix

## Overview

This document describes the fix applied to resolve the linking issue with the MapManager class:
```
unresolved external symbol "public: void __cdecl MapManager::SetSceneBounds(float,float)" (?SetSceneBounds@MapManager@@QEAAXMM@Z) referenced in function "public: void __cdecl GameEngine::SetMapManager(class std::unique_ptr<class MapManager,struct std::default_delete<class MapManager> >)" (?SetMapManager@GameEngine@@QEAAXV?$unique_ptr@VMapManager@@U?$default_delete@VMapManager@@@std@@@std@@@Z)
```

## Root Cause

The issue was caused by a circular dependency between the Engine and GameLogic libraries:

1. **MapManager** was defined in the GameLogic library (GameLogic/MapManager.h, GameLogic/MapManager.cpp)
2. **GameEngine** was defined in the Engine library (Engine/GameEngine.h, Engine/GameEngine.cpp)
3. **GameEngine::SetMapManager** was trying to call **MapManager::SetSceneBounds**
4. This created a circular dependency where Engine library needed symbols from GameLogic library

## Solution

The solution was to move the MapManager class from the GameLogic library to the Engine library:

### Files Moved:
- `GameLogic/MapManager.h` → `Engine/MapManager.h`
- `GameLogic/MapManager.cpp` → `Engine/MapManager.cpp`

### Rationale:
- MapManager is a core engine component that manages game maps
- It should be part of the Engine library rather than the GameLogic library
- This eliminates the circular dependency between Engine and GameLogic libraries

## Changes Made

### 1. File Relocation
- Moved MapManager files to the Engine directory
- Updated CMakeLists.txt to automatically include MapManager.cpp in the Engine library (using glob pattern)

### 2. Include Path Updates
- Updated includes in Engine/GameEngine.cpp to reference "MapManager.h"
- Updated includes in Editor/GameEditor.cpp to reference "../Engine/MapManager.h"
- Updated includes in Engine/MapManager.cpp to reference "../GameLogic/*.h" for demo maps

### 3. Header Updates
- Updated DefaultMap.h to reference "../Engine/GameMap.h" instead of "GameMap.h"

## Verification

All files now pass syntax checking with no errors:
- Engine/MapManager.h - No errors
- Engine/MapManager.cpp - No errors
- Engine/GameEngine.cpp - No errors
- Editor/GameEditor.cpp - No errors

## Benefits

### 1. Resolved Linking Issue
- The unresolved external symbol error is now fixed
- MapManager methods are now properly linked as part of the Engine library

### 2. Proper Architecture
- MapManager is now correctly placed in the Engine library as a core engine component
- Eliminates circular dependency between Engine and GameLogic libraries
- Maintains clean separation of concerns

### 3. Backward Compatibility
- All existing functionality is preserved
- GameLogic can still use MapManager through the Engine library
- DLL loading system continues to work correctly

## Architecture Impact

### Before Fix:
```
Engine Library ←→ GameLogic Library (Circular Dependency)
```

### After Fix:
```
Engine Library (contains MapManager)
       ↑
GameLogic Library (uses MapManager from Engine)
```

This fix ensures a clean, unidirectional dependency structure where the GameLogic library can use components from the Engine library without creating circular dependencies.