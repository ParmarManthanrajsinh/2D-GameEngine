# RayWaves Game Engine - User Guide

Welcome to RayWaves Game Engine! This is a powerful 2D game development platform that combines a visual editor with hot-reloadable game logic, perfect for rapid game development and prototyping.

## 🚀 Quick Start - Getting Your Engine Running

### Step 1: Set Up Your Development Environment
1. **Open Visual Studio Code**
2. **Open the engine directory** in VS Code (File → Open Folder → Select your engine folder)
3. **Open the integrated terminal** by pressing `Ctrl + ` (backtick) or going to Terminal → New Terminal

### Step 2: Launch the Engine
In the VS Code terminal, type:
```bash
./editor.exe
```

That's it! The RayWaves engine editor will launch with:
- **Scene View**: Real-time preview of your game world
- **Control Panel**: Play, pause, restart, and compilation controls
- **Map Selector**: Switch between different game levels
- **Settings Panel**: Adjust scene resolution and export options

## 🎮 Understanding the Engine

### What You Get
- **Visual Editor**: See your game running in real-time
- **Hot Reload**: Modify code and see changes instantly without restarting
- **Built-in Tools**: Particle systems, map management, asset loading
- **Export System**: Create standalone game distributions

### Project Structure Overview
```
YourEngine/
├── editor.exe          # Main engine/editor (run this!)
├── GameLogic.dll       # Your game code (auto-reloads)
├── raylib.dll          # Graphics library
├── config.ini          # Game settings
├── build_gamelogic.bat # Build script for your code
├── GameLogic/          # Your game source code (edit these!)
├── Engine/             # Engine headers (reference only)
├── Assets/             # Your textures, sounds, etc.
└── Documentation/      # This guide and more
```

## 🛠️ Development Workflow

### Making Your First Changes
1. **With the engine running**, open any file in the `GameLogic/` folder (try `Level1.cpp`)
2. **Make a small change** - maybe modify some text or a number
3. **Save the file** (`Ctrl+S`)
4. **In the terminal**, run the build script:
   ```bash
   ./build_gamelogic.bat
   ```
5. **Watch the magic!** Your changes appear in the running engine automatically

No more waiting for full rebuilds or losing your game state!

## 🎯 Core Development Concepts

### Game Maps/Levels
Your game is organized into "maps" (levels or scenes). Each map is a C++ class that handles:
- **Init()**: Set up your level
- **Update()**: Game logic that runs every frame  
- **Draw()**: What appears on screen
- **Reset()**: What happens when restarting

### Example: Basic Level Structure
```cpp
class MyLevel : public GameMap {
public:
    void Init() override {
        // Set up your level here
        player_x = 100;
        player_y = 100;
    }
    
    void Update(float deltaTime) override {
        // Game logic goes here
        if (IsKeyDown(KEY_RIGHT)) player_x += 200 * deltaTime;
        if (IsKeyDown(KEY_LEFT))  player_x -= 200 * deltaTime;
    }
    
    void Draw() override {
        // Drawing code here
        DrawRectangle(player_x, player_y, 32, 32, RED);
        DrawText("Use arrow keys to move", 10, 10, 20, BLACK);
    }
    
    void Reset() override {
        // Reset level state
        player_x = 100;
        player_y = 100;
    }

private:
    float player_x, player_y;
};
```

### Multiple Levels
You can create multiple levels and switch between them in the editor:
- Create new `.cpp/.h` files in `GameLogic/`
- Implement the `GameMap` interface
- The engine automatically detects and lists them

### Built-in Features You Can Use
- **Particle Systems**: `FireParticle` for effects
- **Input Handling**: `IsKeyDown()`, `IsMouseButtonPressed()`, etc.
- **Asset Loading**: `LoadTexture()`, `LoadSound()`, `LoadFont()`
- **Drawing Functions**: `DrawTexture()`, `DrawText()`, `DrawRectangle()`, etc.

## ⚙️ Configuration and Settings

### Game Configuration (`game_config.ini`)
Adjust your game's basic settings:
```ini
width=1280          # Window width in pixels
height=720          # Window height in pixels
b_Fullscreen=false  # Start in fullscreen?
b_Resizable=true    # Allow window resizing?
b_Vsync=true        # Smooth rendering?
target_fps=60       # Target framerate
title=RayWaves Game # Window title
```

### Editor Controls
- **Play Button**: Start/resume game logic
- **Pause Button**: Freeze game logic
- **Restart Button**: Reset current level
- **Compile Button**: Manual rebuild (same as running build script)
- **Map Dropdown**: Switch between different levels
- **Export Panel**: Package your finished game

## 📁 Working with Assets

### Adding Your Own Assets
1. **Place files** in the `Assets/` folder:
   ```
   Assets/
   ├── textures/
   │   ├── player.png
   │   └── background.jpg
   ├── sounds/
   │   ├── jump.wav
   │   └── music.ogg
   └── fonts/
       └── game_font.ttf
   ```

2. **Load them in your code**:
   ```cpp
   Texture2D playerTexture = LoadTexture("Assets/textures/player.png");
   Sound jumpSound = LoadSound("Assets/sounds/jump.wav");
   Font gameFont = LoadFont("Assets/fonts/game_font.ttf");
   ```

3. **Rebuild** your game logic to use them

### Supported File Formats
- **Images**: PNG, JPG, BMP, TGA
- **Audio**: WAV, MP3, OGG
- **Fonts**: TTF, OTF

## 📦 Exporting Your Game

When you're ready to share your creation:

1. **Click "Export Game"** in the editor's export panel
2. **Configure settings**:
   - Choose output folder and game name
   - Set window size and display options
   - Adjust performance settings
3. **Click "Start Export"**
4. **Wait for completion** - you'll get a standalone game folder

The exported game includes everything needed to run on other computers!

## 🔧 Troubleshooting

### "Build failed" Error
**If `build_gamelogic.bat` fails:**
- Make sure you opened the folder in VS Code properly
- Check that you have Visual Studio or Build Tools installed
- Try running VS Code as administrator

### Changes Not Appearing
**If hot reload isn't working:**
- Verify the build script completed successfully
- Check that `GameLogic.dll` file timestamp updated
- Try pausing and playing in the editor

### Performance Issues
**If the engine runs slowly:**
- Check your `Update()` functions aren't doing too much work
- Reduce particle effects if using them
- Adjust settings in `game_config.ini`

### Can't Find Files
**If assets won't load:**
- Double-check file paths (use forward slashes: `"Assets/player.png"`)
- Ensure file names match exactly (case-sensitive)
- Verify files are in the correct folders

## 📚 Learning Path

### Beginner Steps
1. **Run the engine** and explore the sample levels
2. **Make small edits** to `Level1.cpp` and see them change
3. **Add simple features** like new colors, shapes, or text
4. **Try adding movement** or simple interactions

### Intermediate Projects
1. **Create a new level** by copying and modifying existing ones
2. **Add your own assets** (images and sounds)
3. **Implement simple game mechanics** (scoring, collision)
4. **Experiment with particles** for visual effects

### Advanced Development
1. **Study the engine headers** in the `Engine/` folder
2. **Create complex interactions** between multiple objects
3. **Build complete game systems** (menus, multiple levels)
4. **Export and distribute** your finished games

## 🎯 Pro Tips for Success

### Development Best Practices
- **Save frequently** and test changes immediately
- **Use descriptive variable names** for easier debugging
- **Start simple** and add complexity gradually
- **Comment your code** so you remember what it does

### Hot Reload Workflow
- **Keep the engine running** while developing
- **Use the pause button** when making major changes
- **Test frequently** with the play/pause controls
- **Save all files** before rebuilding

### Asset Organization
- **Use subfolders** in Assets/ to stay organized
- **Keep file names simple** and without spaces
- **Test asset loading** immediately after adding new files

## 🆘 Need More Help?

### Additional Documentation
- **[Developer Guide](DEVELOPER_GUIDE.md)** - Advanced engine features
- **[API Reference](API_REFERENCE.md)** - Complete function listings
- **[Particle System Guide](PARTICLE_SYSTEM_GUIDE.md)** - Effects and animations
- **[Troubleshooting Guide](TROUBLESHOOTING.md)** - Detailed problem solving

### Quick Reference Commands
```bash
# Start the engine
./editor.exe

# Rebuild your game logic
./build_gamelogic.bat

# Alternative build (without pause)
./build_gamelogic.bat nopause
```

---
