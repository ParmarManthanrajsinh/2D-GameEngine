#include "../Engine/MapManager.h"
#include "DefaultMap.h"
#include "TestMap.h"

extern "C" __declspec(dllexport) GameMap* CreateGameMap()
{
    // Create and configure your MapManager
    MapManager* manager = new MapManager();
    
    // Register your game maps
    // These are just default - replace with your actual maps
    manager->RegisterMap<DefaultMap>("Default");
    
    manager->RegisterMap<TestMap>("test");

    // Automatically load the first registered map
    auto availableMaps = manager->GetAvailableMaps();
    if (!availableMaps.empty()) {
        manager->GoToMap(availableMaps[0]);
    }
    
    
    return manager;
}