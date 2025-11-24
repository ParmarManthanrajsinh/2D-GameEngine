#include "../Engine/MapManager.h"
#include "Level1.h"
#include "Level2.h"
#include <memory>

// Global static instance to ensure consistency across editor and runtime
static MapManager* g_GameMapManager = nullptr;

extern "C" __declspec(dllexport) GameMap* CreateGameMap()
{
    // If we already have a manager, reuse it to maintain map registrations
    if (g_GameMapManager == nullptr) 
    {
        g_GameMapManager = new MapManager();
        
        // Register your game maps - this happens only once
        g_GameMapManager->RegisterMap<Level1>("Level1");
        g_GameMapManager->RegisterMap<Level2>("Level2");
    }

    // Automatically load the first registered map
    auto available_maps = g_GameMapManager->GetAvailableMaps();
    if (!available_maps.empty())
    {
        g_GameMapManager->b_GotoMap(available_maps.at(0));
    }
    
    return g_GameMapManager;
}