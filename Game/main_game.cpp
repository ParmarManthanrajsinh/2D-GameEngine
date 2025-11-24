#include "GameEngine.h"
#include "DllLoader.h"
#include <raylib.h>
#include <memory>
#include <iostream>

using CreateGameMapFunc = GameMap* (*)();

static std::unique_ptr<GameMap> LoadGameLogic
(
    const char* dllPath, DllHandle& outHandle
)
{
    outHandle = LoadDll(dllPath);
    if (!outHandle.handle)
    {
        std::cerr << "Failed to load GameLogic DLL: " << dllPath << "\n";
        return nullptr;
    }

    auto CreateFn = reinterpret_cast<CreateGameMapFunc>
    (
        GetDllSymbol(outHandle, "CreateGameMap")
    );
    if (!CreateFn)
    {
        std::cerr << "Failed to find symbol CreateGameMap in GameLogic DLL\n";
        UnloadDll(outHandle);
        outHandle = {nullptr, {}};
        return nullptr;
    }

    GameMap* raw = CreateFn();
    if (!raw)
    {
        std::cerr << "CreateGameMap returned null\n";
        UnloadDll(outHandle);
        outHandle = {nullptr, {}};
        return nullptr;
    }

    return std::unique_ptr<GameMap>(raw);
}

int main()
{
    std::cout << "Starting game runtime..." << std::endl;

    GameEngine engine;
    engine.LaunchWindow(1280, 720, "My Game");
    SetTargetFPS(60);

    DllHandle gameLogicHandle{nullptr, {}};
    auto map = LoadGameLogic("GameLogic.dll", gameLogicHandle);
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
        float dt = GetFrameTime();
        engine.UpdateMap(dt);

        BeginDrawing();
        ClearBackground(BLACK);
        engine.DrawMap();
        EndDrawing();
    }

    UnloadDll(gameLogicHandle);
    CloseWindow();
    return 0;
}
