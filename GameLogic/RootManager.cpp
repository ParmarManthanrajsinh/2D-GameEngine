#include "../Engine/MapManager.h"
#include "Level1.h"
#include "Level2.h"
#include <memory>
#include <mutex>

// DLL-safe static instance management
namespace 
{
    // Use a raw pointer with manual lifetime management for DLL safety
    MapManager* GameMapManager = nullptr;
    std::once_flag InitFlag;
    std::mutex ManagerMutex;
    
    // Thread-safe initialization
    void InitializeGameMapManager()
    {
        std::lock_guard<std::mutex> lock(ManagerMutex);
        if (!GameMapManager) 
        {
            GameMapManager = new MapManager();
            
            // Register your game maps - this happens only once
            GameMapManager->RegisterMap<Level1>("Level1");
            GameMapManager->RegisterMap<Level2>("Level2");
        }
    }
    
    // Get or create the singleton instance
    MapManager* GetGameMapManagerInstance() 
    {
        std::call_once(InitFlag, InitializeGameMapManager);
        return GameMapManager;
    }
    
    // Safe cleanup for DLL scenarios
    void SafeCleanupGameMapManager() 
    {
        std::lock_guard<std::mutex> lock(ManagerMutex);
        if (GameMapManager) 
        {
            delete GameMapManager;
            GameMapManager = nullptr;
        }
    }
}

extern "C" __declspec(dllexport) GameMap* CreateGameMap()
{
    MapManager* manager = GetGameMapManagerInstance();
    
    // Automatically load the first registered map
    auto available_maps = manager->GetAvailableMaps();
    if (!available_maps.empty())
    {
        manager->b_GotoMap(available_maps.at(0));
    }
    
    return manager;
}

extern "C" __declspec(dllexport) void CleanupGameResources()
{
    SafeCleanupGameMapManager();
}