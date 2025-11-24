#pragma once

#include "GameMap.h"
#include "GameConfig.h"
#include <iostream>
#include <memory>
#include <raylib.h>
class MapManager;

class GameEngine
{
    bool m_bIsRunning = false;
    bool b_IsRunning() const { return m_bIsRunning; }

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
	void LaunchWindow(const WindowConfig& config);
	void ToggleFullscreen();
	void SetWindowMode(bool fullscreen);
	void SetMap(std::unique_ptr<GameMap> game_map);
	void DrawMap() const;
	void UpdateMap(float delta_time) const;
	void ResetMap();
	
	// MapManager integration methods
	void SetMapManager(std::unique_ptr<MapManager> map_manager);
	MapManager* GetMapManager() const;
	bool b_HasMapManager() const;
};