#include "GameEngine.h"

GameEngine::GameEngine()
{
	m_window_width = 1280;
	m_window_height = 720;
	m_window_title = "Game Window";
}

GameEngine::~GameEngine()
{
}

void GameEngine::LaunchWindow(int width, int height, std::string title)
{
	m_window_width = width;
	m_window_height = height;
	m_window_title = title;

	std::cout << "Window initialized: " << title << " (" << width << "x" << height << ")" << std::endl;

	InitWindow(width, height, title.c_str());
}

void GameEngine::SetMap(std::unique_ptr<GameMap> game_map)
{
	m_game_map = std::move(game_map);

}


void GameEngine::DrawMap() const
{
	if(m_game_map)
	{
		m_game_map->Draw();
	}
}

void GameEngine::UpdateMap(float DeltaTime) const
{
	if(m_game_map)
	{
		m_game_map->Update(DeltaTime);
	}
}
