#include "GameEngine.h"
#include "MapManager.h"
#include "GameConfig.h"

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


void GameEngine::LaunchWindow(const t_WindowConfig& config)
{
	m_WindowWidth = config.width;
	m_WindowHeight = config.height;
	m_WindowTitle = config.title;

	std::cout << "Window initialized from config: "
		<< config.title
		<< " ("
		<< config.width
		<< "x"
		<< config.height
		<< ") "
		<< (config.b_Fullscreen ? "Fullscreen" : "Windowed")
		<< std::endl;

	// Set window flags before initialization
	unsigned int flags = 0;
	if (config.b_Resizable) flags |= FLAG_WINDOW_RESIZABLE;
	if (config.b_Vsync) flags |= FLAG_VSYNC_HINT;

	if (flags != 0) 
	{
		SetConfigFlags(flags);
	}

	InitWindow(config.width, config.height, config.title.c_str());

	// Set fullscreen after window creation if needed
	if (config.b_Fullscreen)
	{
		ToggleFullscreen();
	}
}

void GameEngine::ToggleFullscreen()
{
	::ToggleFullscreen();
	if (IsWindowFullscreen())
	{
		std::cout << "Switched to fullscreen mode" << std::endl;
	}
	else
	{
		std::cout << "Switched to windowed mode" << std::endl;
	}
}

void GameEngine::SetWindowMode(bool fullscreen)
{
	bool isCurrentlyFullscreen = IsWindowFullscreen();
	if (fullscreen && !isCurrentlyFullscreen)
	{
		::ToggleFullscreen();
		std::cout << "Switched to fullscreen mode" << std::endl;
	}
	else if (!fullscreen && isCurrentlyFullscreen)
	{
		::ToggleFullscreen();
		std::cout << "Switched to windowed mode" << std::endl;
	}
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