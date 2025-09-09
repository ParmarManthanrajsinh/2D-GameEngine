#include "GameEngine.h"

GameEngine::GameEngine()
{
	m_WindowWidth = 1280;
	m_WindowHeight = 720;
	m_WindowTitle = "Game Window";
}

GameEngine::~GameEngine()
{
}

void GameEngine::LaunchWindow(int width, int height, const char* title)
{
	m_WindowWidth = width;
	m_WindowHeight = height;
	m_WindowTitle = title;

	std::cout << "Window initialized: "
		<< title
		<< " ("
		<< width
		<< "x"
		<< height
		<< ")"
		<< std::endl;

	InitWindow(width, height, title);
}

void GameEngine::SetMap(std::unique_ptr<GameMap> game_map)
{
	m_GameMap = std::move(game_map);
	if (m_GameMap)
	{
		m_GameMap->SetSceneBounds
		(
			static_cast<float>(m_WindowWidth), 
			static_cast<float>(m_WindowHeight)
		);
		m_GameMap->Initialize();
	}
}

void GameEngine::DrawMap() const
{
	if (m_GameMap)
	{
		m_GameMap->Draw();
	}
}

void GameEngine::UpdateMap(float DeltaTime) const
{
	if (m_GameMap)
	{
		m_GameMap->Update(DeltaTime);
	}
}

void GameEngine::ResetMap()
{
	m_GameMap->Initialize();
}