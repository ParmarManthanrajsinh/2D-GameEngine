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
    DllHandle gameLogicDLL = LoadDll("GameLogic.dll");
    if (!gameLogicDLL.handle) 
    {
        std::cerr << "Failed to load GameLogic.dll!" << std::endl;
        return -1;
    }

    // Get the factory function
    typedef GameMap* (*CreateGameMapFunc)();

    auto createGameMap = reinterpret_cast<CreateGameMapFunc>(GetDllSymbol(gameLogicDLL, "CreateGameMap"));

    if (!createGameMap) 
    {
        std::cerr << "Failed to find CreateGameMap in DLL!" << std::endl;
        UnloadDll(gameLogicDLL);
        return -1;
    }

    {
        // Scope to ensure objects are destroyed before unloading the DLL
        std::unique_ptr<GameMap> my_map(createGameMap());

        GameEditor editor;
        editor.Init(1280,720,"My Game");
        editor.LoadMap(my_map); // editor takes ownership internally via std::move
        editor.Run();
    }

    // Unload the DLL after the editor and map are destroyed
    UnloadDll(gameLogicDLL);
    return 0;
}