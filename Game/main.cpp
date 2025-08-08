#include "GameEditor.h"
#include "GameEngine.h"

#include <memory>
#include <iostream>

#include "GameMap.h"
#include "DllLoader.h"

int main()
{
    printf("Game Engine Starting...\n");

    // Load the DLL via helper to avoid Windows headers here
    DllHandle game_logic_dll = LoadDll("GameLogic.dll");
    if (!game_logic_dll.handle) 
    {
        std::cerr << "Failed to load GameLogic.dll!" << std::endl;
        return -1;
    }

    // Get the factory function
    typedef GameMap* (*CreateGameMapFunc)();

    auto CreateGameMap = reinterpret_cast<CreateGameMapFunc>(GetDllSymbol(game_logic_dll, "CreateGameMap"));

    if (!CreateGameMap) 
    {
        std::cerr << "Failed to find CreateGameMap in DLL!" << std::endl;
        UnloadDll(game_logic_dll);
        return -1;
    }

    {
        // Scope to ensure objects are destroyed before unloading the DLL
        std::unique_ptr<GameMap> my_map(CreateGameMap());

        GameEditor editor;
        editor.Init(1280,720,"My Game");
        editor.LoadMap(my_map);
        editor.Run();
    }

    // Unload the DLL after the editor and map are destroyed
    UnloadDll(game_logic_dll);
    return 0;
}