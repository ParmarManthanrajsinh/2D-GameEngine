#include "GameEngine.h"
#include "DllLoader.h"
#include "GameConfig.h"
#include <raylib.h>
#include <memory>
#include <iostream>
using CreateGameMapFunc = GameMap* (*)();

static std::unique_ptr<GameMap> s_fLoadGameLogic
(
    const char* dll_path, DllHandle& out_handle
)
{
    out_handle = LoadDll(dll_path);
    if (!out_handle.handle)
    {
        std::cerr << "Failed to load GameLogic DLL: " << dll_path << "\n";
        return nullptr;
    }

    auto CreateFn = reinterpret_cast<CreateGameMapFunc>
    (
        GetDllSymbol(out_handle, "CreateGameMap")
    );
    if (!CreateFn)
    {
        std::cerr << "Failed to find symbol CreateGameMap in GameLogic DLL\n";
        UnloadDll(out_handle);
        out_handle = {nullptr, {}};
        return nullptr;
    }

    GameMap* raw = CreateFn();
    if (!raw)
    {
        std::cerr << "CreateGameMap returned null\n";
        UnloadDll(out_handle);
        out_handle = {nullptr, {}};
        return nullptr;
    }

    return std::unique_ptr<GameMap>(raw);
}

int main()
{
    std::cout << "Starting game runtime..." << std::endl;

    // Load configuration
    GameConfig& config = GameConfig::GetInstance();
    config.m_bLoadFromFile("config.ini");
    
    GameEngine engine;
    engine.LaunchWindow(config.GetWindowConfig());
    
    // Set FPS based on vsync setting
    if (config.GetWindowConfig().b_Vsync) 
    {
        SetTargetFPS(0); // Let vsync handle it
    }
    else 
    {
        SetTargetFPS(config.GetWindowConfig().target_fps);
    }

    DllHandle game_logic_handle{nullptr, {}};
    auto map = s_fLoadGameLogic("GameLogic.dll", game_logic_handle);
    if (map)
    {
        engine.SetMap(std::move(map));
    }
    else
    {
        std::cerr << "Running without GameLogic (no map loaded)." 
                  << std::endl;
    }

    while (!WindowShouldClose())
    {
        // Handle Alt+Enter for fullscreen toggle
        if (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_ENTER))
        {
            engine.ToggleFullscreen();
        }
        
        float dt = GetFrameTime();
        engine.UpdateMap(dt);

        BeginDrawing();
        ClearBackground(BLACK);
        engine.DrawMap();
        EndDrawing();
    }

    UnloadDll(game_logic_handle);
    CloseWindow();
    return 0;
}
