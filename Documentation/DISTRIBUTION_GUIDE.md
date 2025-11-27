# RayWaves Distribution Guide

*Where code changes flow like waves* 🌊

How to package RayWaves for sharing with other developers and end users.

## Quick Distribution

**Step 1:** Open Developer Command Prompt for VS

**Step 2:** Navigate to your RayWaves project root

**Step 3:** Run the distribution script:
```cmd
Distribution\create_distribution.bat
```

This creates a complete package in the `dist/` folder that others can use for game development.

## What's Included in Distribution

```
dist/
├── app.exe              # RayWaves engine/editor executable
├── raylib.dll          # Graphics library dependency
├── GameLogic.dll       # Hot-reloadable game logic
├── game_config.ini     # Game configuration template
├── build_gamelogic.bat # Quick build script for users
├── CMakeLists.txt      # Build configuration for GameLogic
├── GameLogic/          # Complete source code for game logic
│   ├── RootManager.cpp # DLL entry point
│   ├── Level1.cpp/h    # Example levels
│   ├── Level2.cpp/h    # More examples
│   └── FireParticle.h  # Particle system example
├── Engine/             # Engine headers for development
│   ├── GameEngine.h    # Core engine interface
│   ├── GameMap.h       # Map base class
│   ├── MapManager.h    # Level management
│   └── GameConfig.h    # Configuration system
├── raylib/             # Raylib development files
│   ├── include/        # Raylib headers
│   └── lib/           # Raylib libraries for linking
├── Assets/             # Game assets (excluding EngineContent)
└── Documentation/      # Complete user guides and API reference
```

## Distribution Features

**For End Users:**
- **Complete Development Environment** - Everything needed to create games
- **Hot-Reload Support** - Edit code and see changes instantly
- **Build Scripts** - Simple `build_gamelogic.bat` for compilation
- **Example Code** - Working levels and particle systems
- **Documentation** - User guides, API reference, troubleshooting

**Self-Contained:**
- No need for original source project
- All development headers included
- Raylib dependencies bundled
- Works on systems without development tools installed

## How Users Work with Distribution

### User Workflow
1. **Open VS Code** in the distributed folder
2. **Launch engine** via terminal: `./app.exe`
3. **Edit GameLogic files** using VS Code
4. **Build changes**: `./build_gamelogic.bat`
5. **See changes instantly** in the running engine

### What Users Can Do
- **Create new levels** by copying existing ones
- **Modify game logic** with hot-reload support
- **Add assets** (textures, sounds, fonts)
- **Use particle systems** and built-in features
- **Export finished games** as standalone executables
- **Switch between levels** using the editor

## Advanced Distribution Options

### Custom Distribution Script
Modify `Distribution/distribute.ps1` for custom packaging:
- Change output directory
- Filter specific assets
- Include additional files
- Set custom build configurations

### Distribution Types
- **Debug Distribution** - Includes debug symbols and detailed logging
- **Release Distribution** - Optimized for performance and smaller size
- **Custom Builds** - Modified configurations for specific needs

## Testing Your Distribution

### Verification Steps
1. **Copy `dist/` folder** to another location (or different computer)
2. **Run `app.exe`** - should start with sample levels
3. **Test hot-reload**: 
   - Edit `GameLogic/Level1.cpp`
   - Run `build_gamelogic.bat`
   - Verify changes appear in running engine
4. **Test asset loading** - ensure textures and sounds work
5. **Test export system** - create standalone game
6. **Verify on clean system** - test without development tools

### Common Distribution Issues

**Missing DLL errors:**
- Install Visual C++ Redistributable
- Include additional runtime libraries if needed

**Build script failures:**
- Ensure CMake is available or provide portable version
- Test on systems without Visual Studio installed
- Include build tools if targeting non-developer users

**Asset loading problems:**
- Verify all asset paths are relative
- Check that required assets are included
- Test asset loading on different systems

**Export functionality issues:**
- Ensure config.ini is properly formatted
- Test export on clean systems
- Verify all dependencies are included

## Distribution Best Practices

### Before Distribution
- **Test thoroughly** on multiple systems
- **Clean build** everything from scratch
- **Verify documentation** is up to date
- **Check asset organization** and paths
- **Test hot-reload functionality**

### Documentation for Users
- Include **README_DISTRIBUTION.md** with clear instructions
- Provide **example code** and tutorials
- Document **troubleshooting** steps
- Explain **workflow** clearly

### Support Considerations
- **Version tracking** - include version info
- **Update mechanism** - plan for updates
- **User feedback** - channels for bug reports
- **Documentation maintenance** - keep guides current

---

## Distribution Checklist

- [ ] Distribution builds successfully
- [ ] Hot-reload works in distributed version
- [ ] All example levels function correctly
- [ ] Asset loading works properly
- [ ] Export system creates working games
- [ ] Documentation is complete and accurate
- [ ] Tested on clean system without dev tools
- [ ] Build script works for end users

**Your RayWaves engine distribution is ready to empower other developers! 🎮**