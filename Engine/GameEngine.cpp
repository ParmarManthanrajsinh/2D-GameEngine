#include "GameEngine.h"
#include "MapManager.h"

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
	// First check if we have a MapManager
	// Otherwise, use the regular GameMap
	if (m_MapManager)
	{
		m_MapManager->Draw();
	}
	else if (m_GameMap)
	{
		m_GameMap->Draw();
	}
}

void GameEngine::UpdateMap(float dt) const
{
	if (m_MapManager)
	{
		m_MapManager->Update(dt);
	}
	else if (m_GameMap)
	{
		m_GameMap->Update(dt);
	}
}

void GameEngine::ResetMap()
{
    if (m_GameMap)
    {
        m_GameMap->Initialize();
    }
}

void GameEngine::SetMapManager(std::unique_ptr<MapManager> map_manager)
{
	m_MapManager = std::move(map_manager);
	if (m_MapManager)
	{
		m_MapManager->SetSceneBounds
		(
			static_cast<float>(m_WindowWidth), 
			static_cast<float>(m_WindowHeight)
		);
		m_MapManager->Initialize();
	}
}

MapManager* GameEngine::GetMapManager() const
{
	return m_MapManager.get();
}

bool GameEngine::b_HasMapManager() const
{
	return m_MapManager != nullptr;
}