#pragma once
#include "GameMap.h"
#include <iostream>
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include <functional>

// Forward declaration for DefaultMap
class DefaultMap;

/**
 * @brief Developer-friendly MapManager for easy game map management
 * 
 * This class provides a simple interface for game developers to:
 * - Register maps with a single line of code
 * - Switch between maps easily
 * - Handle map transitions smoothly
 * - Get helpful error messages and debugging info
 * 
 * Example Usage:
 * @code
 * // In your map registration (usually in Initialize):
 * manager.RegisterMap<YourMapClass>("map_id");
 * 
 * // Switch maps anywhere in your code:
 * manager.GoToMap("map_id");
 * 
 * // Check current map:
 * if (manager.IsCurrentMap("map_id")) {
 *     // Do something specific to that map
 * }
 * @endcode
 */
class MapManager : public GameMap
{
private:
    // Current active map
    std::unique_ptr<GameMap> m_CurrentMap;
    
    // Registry of available maps with their factory functions
    std::unordered_map<std::string, std::function<std::unique_ptr<GameMap>()>> m_MapRegistry;
    
    // Current map identifier
    std::string m_CurrentMapId;
    
    // Map metadata for better developer experience
    struct MapInfo {
        std::string description;
        bool isLoaded;
    };
    std::unordered_map<std::string, MapInfo> m_MapInfo;
    
    // Flag to track if default map is being used
    bool m_UsingDefaultMap;

public:
    MapManager();
    ~MapManager() override;
    
    // Explicitly declare move constructor and assignment operator
    MapManager(MapManager&&) noexcept = default;
    MapManager& operator=(MapManager&&) noexcept = default;
    
    // Delete copy constructor and assignment operator
    MapManager(const MapManager&) = delete;
    MapManager& operator=(const MapManager&) = delete;
    
    // Core GameMap interface
    void Initialize() override;
    void Update(float deltaTime) override;
    void Draw() override;
    
    // Scene bounds methods (hiding base class methods)
    void SetSceneBounds(float width, float height);
    Vector2 GetSceneBounds() const;
    
    // ===========================================
    // DEVELOPER-FRIENDLY MAP MANAGEMENT API
    // ===========================================
    
    /**
     * @brief Register a map type with automatic factory creation
     * @tparam T Map class that inherits from GameMap
     * @param mapId Unique identifier for the map
     * @param description Optional description for debugging
     * 
     * Example: RegisterMap<YourMapClass>("map_id", "Description of your map");
     */
    template<typename T>
    void RegisterMap(const std::string& mapId, const std::string& description = "");
    
    /**
     * @brief Switch to a different map by ID
     * @param mapId The map to switch to
     * @param forceReload If true, recreates the map even if it's already loaded
     * @return true if successful, false if map not found or failed to create
     * 
     * Example: GoToMap("boss_fight"); or GoToMap("level_1", true);
     */
    bool GoToMap(const std::string& mapId, bool forceReload = false);
    
    /**
     * @brief Check if a specific map is currently active
     * @param mapId Map ID to check
     * @return true if the specified map is currently active
     */
    bool IsCurrentMap(const std::string& mapId) const;
    
    /**
     * @brief Get the ID of the currently active map
     * @return Current map ID, or empty string if no map is loaded
     */
    const std::string& GetCurrentMapId() const { return m_CurrentMapId; }
    
    /**
     * @brief Get a list of all registered map IDs
     * @return Vector of map IDs that can be used with GoToMap()
     */
    std::vector<std::string> GetAvailableMaps() const;
    
    /**
     * @brief Check if a map is registered
     * @param mapId Map ID to check
     * @return true if the map is registered and can be loaded
     */
    bool IsMapRegistered(const std::string& mapId) const;
    
    /**
     * @brief Unload the current map (useful for cleanup or memory management)
     */
    void UnloadCurrentMap();
    
    /**
     * @brief Reload the current map (useful for testing or resetting state)
     * @return true if successful, false if no current map or reload failed
     */
    bool ReloadCurrentMap();
    
    /**
     * @brief Get debug information about the MapManager state
     * @return String with current map info, registered maps, etc.
     */
    std::string GetDebugInfo() const;
    
    /**
     * @brief Set up common maps for quick development
     * This automatically registers common maps for development
     */
    void SetupDefaultMaps();
    
private:
    /**
     * @brief Handle input for demonstration purposes
     * Shows how easy map switching can be
     */
    void HandleDemoInput();
    
    /**
     * @brief Draw helpful overlay information for the demo
     */
    void DrawDemoOverlay();
    
    /**
     * @brief Load the default fallback map
     */
    void LoadDefaultMap();
};

// Template implementation
template<typename T>
void MapManager::RegisterMap(const std::string& mapId, const std::string& description)
{
    static_assert(std::is_base_of_v<GameMap, T>, "Map type must inherit from GameMap");
    
    // Create factory function that creates instances of type T
    m_MapRegistry[mapId] = []() -> std::unique_ptr<GameMap> {
        return std::make_unique<T>();
    };
    
    // Store metadata
    m_MapInfo[mapId] = { description.empty() ? "No description" : description, false };
    
    std::cout << "[MapManager] Registered map: '" << mapId << "' - " << description << std::endl;
}

// ===========================================
// CONVENIENCE MACROS FOR EVEN EASIER USAGE
// ===========================================

/**
 * @brief Quick macro to register a map with the same name as the class
 * Usage: REGISTER_MAP(manager, YourMapClass);
 */
#define REGISTER_MAP(manager, MapClass) \
    (manager).RegisterMap<MapClass>(#MapClass)

/**
 * @brief Quick macro to register a map with custom ID and description
 * Usage: REGISTER_MAP_AS(manager, YourMapClass, "map_id", "Map description");
 */
#define REGISTER_MAP_AS(manager, MapClass, mapId, desc) \
    (manager).RegisterMap<MapClass>(mapId, desc)

// Removed CreateGameMap() and CreateGameMapCompatible() function declarations from Engine library
// These functions should only be in the GameLogic DLL to avoid symbol conflicts
