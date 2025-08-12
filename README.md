## 2D-GameEngine (Windows) — Hot-reloadable GameLogic

This Raylib + ImGui editor loads your game code from `GameLogic.dll` and supports rebuilding and hot‑reloading while the editor is running.

### Why this is needed
- Windows locks a DLL when loaded. Rebuilding `GameLogic.dll` used to fail or show no changes because the file was locked by `main.exe`.
- The loader now loads a unique shadow copy of the DLL, keeping the original unlocked for rebuilds.

### How hot‑reload works
- `Game/DllLoader`: copies `GameLogic.dll` to a unique temp filename and loads that copy; deletes it on unload.
- `Editor/GameEditor`:
  - `LoadGameLogic(path)`: loads the DLL and creates a new `GameMap`.
  - `ReloadGameLogic()`: destroys the current map, unloads old DLL, loads new DLL, creates a fresh map.
  - Checks the timestamp of `GameLogic.dll` about every 0.5s; if changed, reloads automatically.
  - The Restart toolbar button also forces a reload.

Reloading recreates the `GameMap` instance, so its runtime state resets.

---

## Build and run

Requirements
- Windows + Visual Studio Build Tools (MSVC), CMake 3.10+, Ninja.
- Use a “Developer Command Prompt for VS” so `cl.exe` is on PATH.

Commands

PowerShell:

```
cmake --preset x64-debug
cmake --build out/build/x64-debug --config Debug --target main
out/build/x64-debug/main.exe
```

Assets are copied to the build directory during configure.

---

## Rebuild GameLogic while editor is running

PowerShell:

```
cmake --build out/build/x64-debug --config Debug --target GameLogic
```

- The editor auto‑reloads within ~0.5s or you can click Restart.
- To rebuild `main`, close the app first (otherwise LNK1168 because `main.exe` is in use).

---

## Project layout
- `Engine/`: core engine (`GameEngine`, `GameMap`)
- `Editor/`: ImGui editor and glue
- `Game/`: program entry and DLL loader wrapper
- `GameLogic/`: your game code (built as `GameLogic.dll`)
- `Assets/`: images, fonts, etc.

---

## Troubleshooting  

### LNK1168: cannot open main.exe for writing
Close the app (stop debugging) before rebuilding `main`. Rebuilding `GameLogic` is safe while running.

### Access violation on exit
Happens if a map object outlives the DLL. The editor now destroys the map before unloading the DLL and frees GPU resources before closing the window.

### Compiler not found
Use a Developer Command Prompt for VS so `cl.exe` is on PATH before running CMake.

### Filesystem deprecation warnings
Replaced `std::filesystem::u8path` with `std::filesystem::path`. Clean rebuild if warnings persist.

### Type conversion/member init warnings in `MyMap`
- Initialize members (e.g., `Vector2 position{0.0f, 0.0f};`).
- Prefer `DrawCircleV(...)` or cast explicitly when calling integer overloads.

---

## CMake notes
- `main` links against `Engine` (which links Raylib).
- After building `main`, CMake copies `GameLogic.dll` and Raylib’s runtime DLL next to the exe.
- Building only `GameLogic` updates the DLL the editor watches and reloads.

