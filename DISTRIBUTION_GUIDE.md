# Distribution Guide for 2D Game Engine

This guide will help you create a distribution package of your Raylib-based game engine/editor for sharing with other developers.

## Prerequisites

Before creating a distribution, ensure you have:

1. **Visual Studio Build Tools** or **Visual Studio** installed
2. **CMake 3.10+** installed
3. Your project builds successfully in **Release** mode

## Quick Distribution (Recommended)

### Method 1: Using Batch Script (Easiest)

1. Open **Developer Command Prompt for VS** from Start Menu
   - Search for "Developer Command Prompt for VS 2022" (or your VS version)
   - Or use "x64 Native Tools Command Prompt for VS"

2. Navigate to your project directory:
   ```cmd
   cd path\to\your\2D-GameEngine
   ```

3. Run the distribution script:
   ```cmd
   create_distribution.bat
   ```

The script will:
- Check for Visual Studio environment
- Build release version if needed
- Create complete distribution in `dist/` folder

### Method 2: Using PowerShell

1. Open **Developer Command Prompt for VS**
2. Navigate to project directory
3. Build release version:
   ```cmd
   cmake --preset x64-release
   cmake --build out/build/x64-release --config Release --target main
   ```
4. Run distribution script:
   ```powershell
   .\distribute.ps1
   ```

## Manual Distribution Process

If you prefer to create the distribution manually:

### 1. Build Release Version
```cmd
cmake --preset x64-release
cmake --build out/build/x64-release --config Release --target main
```

### 2. Create Distribution Structure
```
distribution/
├── app.exe                 # Your main executable
├── raylib.dll             # Raylib runtime
├── GameLogic.dll          # Hot-reloadable game logic
├── CMakeLists.txt         # For building GameLogic
├── build_gamelogic.bat    # Quick build helper
├── README_DISTRIBUTION.md # User instructions
├── GameLogic/             # Source code
├── Engine/                # Engine headers
└── Assets/                # Game assets
```

### 3. Copy Files
```cmd
REM Create directories
mkdir dist\GameLogic
mkdir dist\Engine
mkdir dist\Assets

REM Copy executables and DLLs
copy out\build\x64-release\main.exe dist\app.exe
copy out\build\x64-release\raylib.dll dist\
copy out\build\x64-release\GameLogic.dll dist\

REM Copy source and headers
xcopy GameLogic\* dist\GameLogic\ /s /e
xcopy Engine\*.h dist\Engine\
xcopy Assets\* dist\Assets\ /s /e

REM Copy build files
copy dist_CMakeLists.txt dist\CMakeLists.txt
copy build_gamelogic.bat dist\
copy README_DISTRIBUTION.md dist\
```

## What's Included in Distribution

### Core Files
- **app.exe** - The main game engine/editor with ImGui interface
- **raylib.dll** - Shared Raylib library (ensures same version for hot-reloading)
- **GameLogic.dll** - Your initial game logic (hot-reloadable)

### Development Environment
- **CMakeLists.txt** - Configured for building GameLogic only
- **build_gamelogic.bat** - Quick build script for users
- **GameLogic/** - Complete source code for game development
- **Engine/** - Engine header files needed for GameLogic development

### Assets and Documentation
- **Assets/** - All game assets (textures, shaders, icons)
- **README_DISTRIBUTION.md** - Complete user guide

## Distribution Features

### For End Users
- **Simple Setup** - Just run `app.exe`
- **Hot-Reloading** - Edit code and see changes instantly
- **Pure Raylib** - Familiar Raylib coding patterns
- **Visual Editor** - ImGui-based editor interface

### For Developers
- **Easy Building** - Run `build_gamelogic.bat` or use CMake
- **Fast Iteration** - Hot-reload within 0.5 seconds
- **Asset Management** - Integrated file browser
- **Multiple Maps** - Easy scene switching

## Testing Your Distribution

Before sharing, test the distribution:

1. **Copy** the `dist/` folder to a different location
2. **Run** `app.exe` - should start without issues
3. **Test Hot-Reload**:
   - Open Developer Command Prompt in distribution folder
   - Run `build_gamelogic.bat`
   - Verify the engine reloads automatically

## Distribution Size

Typical distribution size:
- **Executables**: ~5-10 MB
- **Source Code**: ~1-2 MB
- **Assets**: Variable (depends on your assets)
- **Total**: Usually 10-20 MB

## Troubleshooting Distribution

### "Missing MSVCR/VCRUNTIME dll" Errors
- Include Visual C++ Redistributable with your distribution
- Or instruct users to install "Microsoft Visual C++ Redistributable"

### "raylib.dll not found"
- Ensure `raylib.dll` is in the same folder as `app.exe`
- The distribution script should handle this automatically

### Build Errors in Distribution
- Test building in a clean environment
- Ensure all necessary headers are included
- Verify CMakeLists.txt paths are correct

## Advanced Options

### Custom Build Configurations
Modify `distribute.ps1` parameters:
```powershell
.\distribute.ps1 -BuildConfig "Debug" -OutputDir "debug_dist"
```

### Including Debug Symbols
For debugging distributions, include `.pdb` files:
- Copy `main.pdb`, `raylib.pdb` alongside the executables

### Packaging for Release
Consider using tools like:
- **NSIS** for Windows installers
- **7-Zip** for compressed archives
- **ZIP** for simple distribution

## Best Practices

1. **Test on Clean Systems** - Verify your distribution works on computers without development tools
2. **Version Control** - Tag releases and keep distribution scripts in version control
3. **Documentation** - Keep README_DISTRIBUTION.md updated with any custom instructions
4. **Asset Organization** - Keep assets organized and well-documented
5. **Build Automation** - Consider CI/CD for automatic distribution creation

Your 2D Game Engine is now ready for distribution! Users can start developing games immediately with hot-reloading capabilities while maintaining the simplicity of Raylib.