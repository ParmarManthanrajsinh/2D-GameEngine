#include "GameMap.h"

GameMap::GameMap()
	: m_MapName("DefaultMap")
{
}

GameMap::GameMap(const std::string& map_name)
	: m_MapName(map_name)
{
}

void GameMap::Initialize()
{
    // Initialization code for the game map
}

void GameMap::Update(float DeltaTime)
{
    // Update logic for the game map
}

void GameMap::Draw()
{
    // Drawing logic for the game map
}

void GameMap::SetMapName(const std::string& map_name)
{
    m_MapName = map_name;
}

std::string GameMap::GetMapName() const
{
    return m_MapName;
}

void GameMap::SetSceneBounds(float width, float height)
{
    m_SceneWidth = width;
    m_SceneHeight = height;
}

Vector2 GameMap::GetSceneBounds() const
{
	return Vector2(m_SceneWidth, m_SceneHeight);
}
