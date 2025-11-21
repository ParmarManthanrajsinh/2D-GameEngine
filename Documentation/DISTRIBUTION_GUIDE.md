# Distribution Guide

This guide shows you how to create a distribution package of your game engine for sharing with other developers.

## 📋 Prerequisites

- Visual Studio Build Tools or Visual Studio
- CMake 3.10+
- Your project builds successfully in **Release** mode

## 🚀 Quick Distribution

### Method 1: Using Batch Script (Easiest)

1. **Open Developer Command Prompt for VS**
   - Search for "Developer Command Prompt for VS 2022" in Start Menu
   - Or use "x64 Native Tools Command Prompt for VS"

2. **Navigate to your project:**
   ```cmd
   cd path\to\your\2D-GameEngine
   ```

3. **Run the distribution script:**
   ```cmd
   create_distribution.bat
   ```

The script will automatically:
- Check for Visual Studio environment
- Build release version if needed
- Create complete distribution in `dist/` folder

### Method 2: Using PowerShell

```cmd
# Open Developer Command Prompt for VS
cd path\to\your\2D-GameEngine

# Build release version
cmake --preset x64-release
cmake --build out/build/x64-release --config Release --target main

# Create distribution
.\distribute.ps1
```

## 📦 What's Included

Your distribution package will contain:

```
dist/
├── app.exe                 # Main engine/editor
├── raylib.dll             # Graphics library
├── GameLogic.dll          # Hot-reloadable game logic
├── CMakeLists.txt         # Build configuration
├── build_gamelogic.bat    # Quick build helper
├── README_DISTRIBUTION.md # User instructions
├── GameLogic/             # Source code
├── Engine/                # Engine headers
└── Assets/                # Game assets
```

### Core Files
- **app.exe** - The game engine/editor with ImGui interface
- **raylib.dll** - Shared Raylib library
- **GameLogic.dll** - Your initial game logic

### Development Environment
- **CMakeLists.txt** - Configured for building GameLogic only
- **build_gamelogic.bat** - Quick build script
- **GameLogic/** - Complete source code
- **Engine/** - Engine headers needed for development

### Assets and Documentation
- **Assets/** - All game assets
- **README_DISTRIBUTION.md** - Complete user guide

## ✅ Testing Your Distribution

Before sharing, test the distribution:

1. **Copy** the `dist/` folder to a different location
2. **Run** `app.exe` - should start without issues
3. **Test Hot-Reload**:
   - Open Developer Command Prompt in distribution folder
   - Run `build_gamelogic.bat`
   - Verify the engine reloads automatically

## 📏 Distribution Size

Typical distribution size:
- **Executables**: ~5-10 MB
- **Source Code**: ~1-2 MB
- **Assets**: Variable (depends on your assets)
- **Total**: Usually 10-20 MB

## 🔧 Troubleshooting

### "Missing MSVCR/VCRUNTIME dll" Errors
**Solution:** Include Visual C++ Redistributable with your distribution or instruct users to install it.

### "raylib.dll not found"
**Solution:** Ensure `raylib.dll` is in the same folder as `app.exe`. The distribution script handles this automatically.

### Build Errors in Distribution
**Solution:** 
- Test building in a clean environment
- Ensure all necessary headers are included
- Verify CMakeLists.txt paths are correct

## 🎁 Packaging for Release

Consider using:
- **7-Zip** for compressed archives
- **ZIP** for simple distribution
- **NSIS** for Windows installers

## 💡 Best Practices

1. **Test on Clean Systems** - Verify your distribution works on computers without development tools
2. **Version Control** - Tag releases and keep distribution scripts in version control
3. **Documentation** - Keep README_DISTRIBUTION.md updated
4. **Asset Organization** - Keep assets organized and well-documented

---

Your 2D Game Engine is now ready for distribution! Users can start developing games immediately with hot-reloading capabilities.