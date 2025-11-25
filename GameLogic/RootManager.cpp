#include "../Engine/MapManager.h"
#include "Level1.h"
#include "Level2.h"
#include <memory>
#include <mutex>

// DLL-safe static instance management
namespace 
{
    // Use a raw pointer with manual lifetime management for DLL safety
    MapManager* s_GameMapManager = nullptr;
    std::once_flag s_InitFlag;
    std::mutex s_ManagerMutex;
    
    // Thread-safe initialization
    void InitializeGameMapManager()
    {
        std::lock_guard<std::mutex> lock(s_ManagerMutex);
        if (!s_GameMapManager) 
        {
            s_GameMapManager = new MapManager();
            
            // Register your game maps - this happens only once
            s_GameMapManager->RegisterMap<Level1>("Level1");
            s_GameMapManager->RegisterMap<Level2>("Level2");
        }
    }
    
    // Get or create the singleton instance
    MapManager* GetGameMapManagerInstance() 
    {
        std::call_once(s_InitFlag, InitializeGameMapManager);
        return s_GameMapManager;
    }
    
    // Safe cleanup for DLL scenarios
    void SafeCleanupGameMapManager() 
    {
        std::lock_guard<std::mutex> lock(s_ManagerMutex);
        if (s_GameMapManager) 
        {
            delete s_GameMapManager;
            s_GameMapManager = nullptr;
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