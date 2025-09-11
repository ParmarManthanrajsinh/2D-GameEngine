#include "MapManager.h"
#include <sstream>

MapManager::MapManager()
    : m_CurrentMap(nullptr)
    , m_CurrentMapId("")
    , m_UsingDefaultMap(false)
{
    std::cout << "[MapManager] Initialized - ready for map registration" << std::endl;
}

MapManager::~MapManager()
{
    // Smart pointer automatically handles cleanup
    std::cout << "[MapManager] Destroyed - all maps cleaned up" << std::endl;
}

// ===========================================
// CORE GAMELOOP METHODS
// ===========================================

void MapManager::Initialize()
{
    std::cout << "[MapManager] MapManager initialized - waiting for map registration" << std::endl;
    
    // Initialize the current map if we have one
    if (m_CurrentMap)
    {
        // Make sure the map has proper scene bounds
        Vector2 bounds = GameMap::GetSceneBounds();
        m_CurrentMap->SetSceneBounds(bounds.x, bounds.y);
        m_CurrentMap->Initialize();
        
        std::cout << "[MapManager] Successfully initialized with map: '" << m_CurrentMapId << "'" << std::endl;
    }
    else if (!m_MapRegistry.empty())
    {
        // Maps are registered but none are loaded yet
        std::cout << "[MapManager] Maps registered but none loaded yet. Use GoToMap() to load a map." << std::endl;
        // List the registered maps
        std::cout << "[MapManager] Registered maps: ";
        for (const auto& pair : m_MapRegistry)
        {
            std::cout << "'" << pair.first << "' ";
        }
        std::cout << std::endl;
    }
    else
    {
        std::cout << "[MapManager] No maps registered yet. Register maps using RegisterMap<YourMap>()" << std::endl;
    }
}

void MapManager::Update(float deltaTime)
{
    // Update the current map if we have one
    if (m_CurrentMap)
    {
        m_CurrentMap->Update(deltaTime);
    }
}

void MapManager::Draw()
{
    // Draw the current map if we have one
    if (m_CurrentMap)
    {
        m_CurrentMap->Draw();
    }
    else
    {
        // Only draw the helpful message if we're in a valid drawing context
        // Check if the window is ready before drawing
        if (GetWindowHandle() != nullptr)
        {
            // Draw a helpful message when no map is loaded
            DrawRectangle(0, 0, GetSceneBounds().x, GetSceneBounds().y, DARKGRAY);
            DrawText("No map loaded - use GoToMap() to load a map", 50, 50, 20, RED);
            DrawText("Register maps using RegisterMap<YourMap>(\"map_id\")", 50, 100, 16, WHITE);
        }
    }
}

void MapManager::SetSceneBounds(float width, float height)
{
    // Update our own bounds first
    GameMap::SetSceneBounds(width, height);
    
    // Forward to current map if it exists
    if (m_CurrentMap)
    {
        m_CurrentMap->SetSceneBounds(width, height);
    }
}

Vector2 MapManager::GetSceneBounds() const
{
    // If we have a current map, get bounds from it
    if (m_CurrentMap)
    {
        return m_CurrentMap->GetSceneBounds();
    }
    
    // Otherwise return our own bounds
    return GameMap::GetSceneBounds();
}

bool MapManager::GoToMap(const std::string& mapId, bool forceReload)
{
    // Check if map is registered
    if (!IsMapRegistered(mapId))
    {
        std::cerr << "[MapManager] Error: Map '" << mapId << "' is not registered!" << std::endl;
        std::cerr << "[MapManager] Available maps: ";
        for (const auto& availableMap : GetAvailableMaps())
        {
            std::cerr << "'" << availableMap << "' ";
        }
        std::cerr << std::endl;
        return false;
    }
    
    // If it's the same map and we don't want to force reload, just return true
    if (m_CurrentMapId == mapId && !forceReload)
    {
        std::cout << "[MapManager] Map '" << mapId << "' is already loaded" << std::endl;
        return true;
    }
    
    std::cout << "[MapManager] Switching to map: '" << mapId << "'" << std::endl;
    
    try
    {
        // Create the new map
        auto& factory = m_MapRegistry[mapId];
        auto newMap = factory();
        
        if (!newMap)
        {
            std::cerr << "[MapManager] Error: Factory for map '" << mapId << "' returned null!" << std::endl;
            return false;
        }
        
        // Successfully created new map - switch to it
        m_CurrentMap = std::move(newMap);
        m_CurrentMapId = mapId;
        m_MapInfo[mapId].isLoaded = true;
        m_UsingDefaultMap = false; // No longer using default map
        
        // Set up the new map with current scene bounds
        Vector2 bounds = GameMap::GetSceneBounds();
        if (m_CurrentMap) {
            m_CurrentMap->SetSceneBounds(bounds.x, bounds.y);
            m_CurrentMap->Initialize();
        }
        
        std::cout << "[MapManager] Successfully loaded map: '" << mapId << "'" << std::endl;
        std::cout << "[MapManager] Current map ID: '" << m_CurrentMapId << "'" << std::endl;
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[MapManager] Error creating map '" << mapId << "': " << e.what() << std::endl;
        return false;
    }
    catch (...)
    {
        std::cerr << "[MapManager] Unknown error creating map '" << mapId << "'" << std::endl;
        return false;
    }
}

bool MapManager::IsCurrentMap(const std::string& mapId) const
{
    return m_CurrentMapId == mapId && m_CurrentMap != nullptr;
}

std::vector<std::string> MapManager::GetAvailableMaps() const
{
    std::vector<std::string> maps;
    maps.reserve(m_MapRegistry.size());
    
    for (const auto& pair : m_MapRegistry)
    {
        maps.push_back(pair.first);
    }
    
    return maps;
}

bool MapManager::IsMapRegistered(const std::string& mapId) const
{
    return m_MapRegistry.find(mapId) != m_MapRegistry.end();
}

void MapManager::UnloadCurrentMap()
{
    if (m_CurrentMap)
    {
        std::cout << "[MapManager] Unloading map: '" << m_CurrentMapId << "'" << std::endl;
        
        // Mark as not loaded in metadata
        if (m_MapInfo.find(m_CurrentMapId) != m_MapInfo.end())
        {
            m_MapInfo[m_CurrentMapId].isLoaded = false;
        }
        
        m_CurrentMap.reset();
        m_CurrentMapId = "";
        m_UsingDefaultMap = false;
    }
    else
    {
        std::cout << "[MapManager] No map to unload" << std::endl;
    }
}

bool MapManager::ReloadCurrentMap()
{
    if (m_CurrentMapId.empty())
    {
        std::cout << "[MapManager] No current map to reload" << std::endl;
        return false;
    }
    
    std::string mapToReload = m_CurrentMapId;
    std::cout << "[MapManager] Reloading map: '" << mapToReload << "'" << std::endl;
    
    return GoToMap(mapToReload, true);
}

std::string MapManager::GetDebugInfo() const
{
    std::stringstream ss;
    ss << "=== MapManager Debug Info ===\n";
    ss << "Current Map: " << (m_CurrentMapId.empty() ? "None" : m_CurrentMapId) << "\n";
    ss << "Using Default Map: " << (m_UsingDefaultMap ? "Yes" : "No") << "\n";
    ss << "Registered Maps (" << m_MapRegistry.size() << "):";
    
    for (const auto& pair : m_MapInfo)
    {
        const std::string& mapId = pair.first;
        const MapInfo& info = pair.second;
        ss << "\n  - '" << mapId << "': " << info.description;
        ss << " [" << (info.isLoaded ? "LOADED" : "NOT LOADED") << "]";
    }
    
    if (m_MapRegistry.empty())
    {
        ss << "\n  (No maps registered - call RegisterMap<YourMap>() to register maps)";
    }
    
    return ss.str();
}

void MapManager::SetupDefaultMaps()
{
    std::cout << "[MapManager] SetupDefaultMaps() - No default maps configured in Engine library" << std::endl;
    std::cout << "[MapManager] Register your maps using RegisterMap<YourMap>(\"map_id\")" << std::endl;
}

// ===========================================
// DEMO INPUT HANDLING (Removed to avoid dependencies on Raylib input functions)
// ===========================================

void MapManager::HandleDemoInput()
{
    // Removed demo input handling to avoid dependencies on Raylib input functions
    // GameLogic DLL can implement its own input handling
}

void MapManager::DrawDemoOverlay()
{
    // Removed demo overlay to avoid dependencies on Raylib drawing functions
    // GameLogic DLL can implement its own overlay drawing
}

void MapManager::LoadDefaultMap()
{
    std::cout << "[MapManager] No default map available in Engine library" << std::endl;
    std::cout << "[MapManager] Register and load your own maps using RegisterMap<YourMap>()" << std::endl;
}
