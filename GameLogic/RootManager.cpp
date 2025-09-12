#include "../Engine/MapManager.h"
#include "DefaultMap.h"
#include "TestMap.h"
#include <memory>

extern "C" __declspec(dllexport) GameMap* CreateGameMap()
{
    MapManager* manager = new MapManager();
    
    // Register your game maps
    // These are just default - replace with your actual maps
    manager->RegisterMap<DefaultMap>("Default");
    manager->RegisterMap<TestMap>("test");

    // Automatically load the first registered map
    auto available_maps = manager->GetAvailableMaps();  
    if (!available_maps.empty())
    {
        manager->b_GotoMap(available_maps.at(0));
    }
    
    return manager;
}