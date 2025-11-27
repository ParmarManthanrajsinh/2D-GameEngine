# RayWaves Engine - Troubleshooting

*Where code changes flow like waves* 🌊

Common issues and solutions for both source development and distributed versions.

## Build Issues

### Source Development Issues

**LNK1168: cannot open main.exe for writing**
- **Cause:** main.exe is running  
- **Solution:** Close editor before rebuilding main.exe (GameLogic.dll rebuilds are safe while running)

**Compiler not found**
- **Cause:** MSVC not on PATH  
- **Solution:** Use Developer Command Prompt for VS or ensure Visual Studio is properly installed

**CMake configuration failed**
- **Cause:** Missing Visual Studio or CMake  
- **Solution:** Install Visual Studio with C++ tools and CMake 3.10+

### Distribution Build Issues

**build_gamelogic.bat fails with "compiler not found"**
- **Cause:** VS Code not opened from Developer Command Prompt
- **Solution:** Open VS Code from Developer Command Prompt or ensure build tools are in PATH

**CMake not found in distributed version**
- **Cause:** CMake not installed on user system
- **Solution:** Install CMake or include portable CMake in distribution

**Permission denied errors**
- **Cause:** Antivirus or insufficient permissions
- **Solution:** Run VS Code as administrator, check antivirus exclusions

## Hot-Reload Issues

### DLL not reloading
**Check:** Build completed successfully  
**Check:** GameLogic.dll timestamp changed  
**Solution:** Click Restart button in editor

### Access violation on exit
**Cause:** Memory cleanup issue  
**Solution:** Ensure proper destructor calls, avoid global state

### Hot-reload loops/crashes
**Cause:** Infinite RequestGotoMap calls  
**Solution:** Add conditions to prevent loops
```cpp
if (someCondition && !m_transitionRequested) {
    m_transitionRequested = true;
    RequestGotoMap("NextLevel");
}
```

## Performance Issues

### Low FPS
**Debug:** Add FPS display
```cpp
DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, RED);
```

**Common causes:**
- Too many particles (limit count)
- Heavy operations in Update() (cache expensive calls)
- Excessive draw calls (batch similar operations)

### Memory leaks
**Cause:** Not cleaning resources in Cleanup()  
**Solution:** Free textures, sounds in map Cleanup() method

## Export/Distribution Issues

### Missing DLL errors
**Cause:** Missing Visual C++ Runtime  
**Solution:** Install VC++ Redistributable or include with distribution

### game_config.ini not working
**Cause:** Wrong file path or format  
**Solution:** 
- Ensure `game_config.ini` is next to executable, not in subfolder
- Check file format matches expected structure
- Verify no syntax errors in INI file

### Export process hangs
**Solutions:**
- Close other programs using significant resources
- Check available disk space
- Run export from command line for detailed error messages
- Disable antivirus real-time scanning temporarily

### Distribution doesn't work on target system
**Common causes:**
- Missing Visual C++ Redistributable
- Incompatible Windows version
- Missing DirectX runtime
- Antivirus blocking execution

**Solutions:**
- Include redistributable packages
- Test on multiple Windows versions
- Provide installation instructions

## Editor Issues

### Interface too small (4K displays)
**Solution:** Windows Display Settings → Set scaling to 125-150%

### File browser crashes
**Solutions:**
- Run as Administrator
- Check antivirus settings
- Update Windows

### Editor layout corrupted
**Solution:** Delete imgui.ini file to reset layout

## Development Environment Issues

### VS Code terminal not working properly
**Solution:** Use Developer Command Prompt for VS or ensure proper PATH setup

### IntelliSense errors in VS Code
**Solutions:**
- Install C/C++ extension
- Ensure proper include paths are configured
- Reload VS Code window

### Engine won't start from VS Code terminal
**Common causes:**
- Wrong working directory
- Missing dependencies
- Path issues

**Solutions:**
- Ensure terminal is in correct directory (where app.exe is located)
- Check that all DLL files are present
- Try running from File Explorer first to test basic functionality

## Asset Loading Issues

### Textures not loading
**Solutions:**
- Use forward slashes in paths: `"Assets/player.png"`
- Ensure file names match exactly (case-sensitive)
- Verify files are in correct directories relative to executable
- Check file formats are supported (PNG, JPG, BMP, TGA)

### Sounds not playing
**Solutions:**
- Verify audio file formats (WAV, MP3, OGG)
- Check system audio settings
- Ensure audio files aren't corrupted
- Test with simple WAV files first

### Fonts not rendering
**Solutions:**
- Use TTF or OTF format fonts
- Ensure font files are accessible
- Test with system fonts first
- Check font loading code syntax

## Getting Help

### Information to Include in Bug Reports
- **RayWaves version** and build type (source/distribution)
- **Windows version** (Windows 10/11, build number)
- **Hardware specs** (GPU, RAM)
- **Development environment** (VS Code, Visual Studio version)
- **Exact error messages** (copy full text)
- **Steps to reproduce** (detailed sequence)
- **Expected vs actual behavior**

### Debug Logging Techniques
```cpp
// Basic debug output
std::cout << "[DEBUG] " << message << std::endl;

// Position tracking
std::cout << "Player position: " << m_PlayerPos.x << ", " << m_PlayerPos.y << std::endl;

// State verification
std::cout << "Current map: " << GetMapName() << std::endl;

// Frame rate monitoring
DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, RED);
```

### Common Debug Workflow
1. **Isolate the problem** - minimal reproduction case
2. **Add debug output** - trace execution flow
3. **Check obvious issues** - missing files, typos, path problems
4. **Test on clean build** - rebuild everything from scratch
5. **Compare with working examples** - Level1.cpp, Level2.cpp

### Performance Debugging
```cpp
void Update(float delta_time) override {
    auto start = std::chrono::high_resolution_clock::now();
    
    // Your game logic here
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    if (duration.count() > 1000) { // More than 1ms
        std::cout << "Slow Update: " << duration.count() << " microseconds" << std::endl;
    }
}
```

---

**Remember: Most issues are solvable with careful attention to error messages and clean rebuilds. The hot-reload system makes debugging faster - use it to your advantage! 🌊**