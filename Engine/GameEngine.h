#pragma once

#include <raylib.h>
#include <iostream>
#include <memory>
#include "GameMap.h"

// Forward declaration for MapManager
class MapManager;

class GameEngine
{
    bool m_bIsRunning = false;
    bool bIsRunning() const { return m_bIsRunning; }

	// The window dimensions
    int m_WindowWidth;
	int m_WindowHeight;
	std::string m_WindowTitle;
	std::unique_ptr<GameMap> m_GameMap;
	
	// MapManager instance for advanced map management
	std::unique_ptr<MapManager> m_MapManager;
	
public:
    GameEngine();
    ~GameEngine();

	void LaunchWindow(int width, int height, const char* title);
	void SetMap(std::unique_ptr<GameMap> game_map);
	void DrawMap() const;
	void UpdateMap(float DeltaTime) const;
	void ResetMap();
	
	// MapManager integration methods
	void SetMapManager(std::unique_ptr<MapManager> map_manager);
	MapManager* GetMapManager() const;
	bool HasMapManager() const;
};