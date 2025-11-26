# RayWaves Engine - Troubleshooting

Common issues and solutions.

## Build Issues

### LNK1168: cannot open main.exe for writing
**Cause:** main.exe is running  
**Solution:** Close editor before rebuilding main.exe (GameLogic.dll rebuilds are safe)

### Compiler not found
**Cause:** MSVC not on PATH  
**Solution:** Use Developer Command Prompt for VS

### CMake configuration failed
**Cause:** Missing Visual Studio or CMake  
**Solution:** Install Visual Studio with C++ tools and CMake 3.10+

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

### Config.ini not working
**Cause:** Wrong file path  
**Solution:** Ensure config.ini is next to executable, not in subfolder

### Export process hangs
**Solutions:**
- Close other programs
- Check disk space
- Run export from command line for error details

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

## Getting Help

**Include in bug reports:**
- RayWaves version
- Windows version
- Hardware specs
- Exact error messages
- Steps to reproduce

**Debug logging:**
```cpp
std::cout << "[DEBUG] " << message << std::endl;
```

---

Most issues are solvable with careful attention to error messages and clean rebuilds.