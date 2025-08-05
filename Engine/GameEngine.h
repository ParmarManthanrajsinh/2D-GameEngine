#pragma once

#include <raylib.h>
#include <iostream>
#include "GameMap.h"

class GameEngine
{
    bool m_bIsRunning = false;
    bool bIsRunning() const { return m_bIsRunning; }

	// The window dimensions
    int m_WindowWidth;
	int m_WindowHeight;
	std::string m_WindowTitle;

	std::unique_ptr<GameMap> m_GameMap;
public:

    GameEngine();
    ~GameEngine();

	void LaunchWindow(int width, int height, std::string title);
	void SetMap(std::unique_ptr<GameMap> game_map);
	void DrawMap() const;
	void UpdateMap(float DeltaTime) const;
};
