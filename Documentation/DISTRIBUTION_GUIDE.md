# RayWaves Distribution Guide

How to package RayWaves for sharing with other developers.

## Quick Distribution

**Step 1:** Open Developer Command Prompt for VS

**Step 2:** Navigate to your RayWaves project

**Step 3:** Run the distribution script:
```cmd
Distribution\create_distribution.bat
```

This creates a complete package in the `dist/` folder.

## What's Included

```
dist/
├── app.exe               # RayWaves engine/editor
├── raylib.dll           # Graphics library  
├── GameLogic.dll        # Hot-reloadable game logic
├── config.ini           # Game configuration
├── build_gamelogic.bat  # Quick build script
├── GameLogic/           # Source code
├── Engine/              # Engine headers
└── Assets/              # Game assets
```

## Testing

1. Copy `dist/` folder to another location
2. Run `app.exe` - should start normally
3. Test hot-reload with `build_gamelogic.bat`

## Common Issues

**Missing dll errors:** Install Visual C++ Redistributable

**Build errors:** Ensure all headers included, test on clean system

---

Your RayWaves engine is ready for distribution!