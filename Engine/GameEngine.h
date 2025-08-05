#pragma once

#include <raylib.h>
#include <iostream>
#include "GameMap.h"

class GameEngine
{
    bool m_bIsRunning = false;
    bool b_IsRunning() const { return m_bIsRunning; }

	// The window dimensions
    int m_window_width;
	int m_window_height;
	std::string m_window_title;

	std::unique_ptr<GameMap> m_game_map;
public:

    GameEngine();
    ~GameEngine();

	void LaunchWindow(int width, int height, std::string title);
	void SetMap(std::unique_ptr<GameMap> game_map);
	void DrawMap() const;
	void UpdateMap(float DeltaTime) const;
};
