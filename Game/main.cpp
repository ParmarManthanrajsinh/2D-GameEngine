#include "GameEditor.h"
#include "GameEngine.h"
#include "GameMap.h"

// DLL loading now handled by GameEditor for hot-reload
int main()
{
    printf("Game Engine Starting...\n");
    GameEditor editor;
    editor.Init(1280,720,"My Game");

    // Load logic DLL and create the map (will show default map if load fails)
    editor.LoadGameLogic("GameLogic.dll");
    editor.Run();
    return 0;
}