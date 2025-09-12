#pragma once
#include <iostream>
#include <raylib.h>
#include <string>

class GameMap
{
protected:
    std::string m_MapName;
    float m_SceneWidth = 0.0f;   
    float m_SceneHeight = 0.0f;  

public:
    GameMap(); 
    GameMap(const std::string& map_name);
    virtual ~GameMap() = default;  

    virtual void Initialize();
    virtual void Update(float delta_time);
    virtual void Draw();
    
    void SetMapName(const std::string& map_name);
    std::string GetMapName() const;
    void SetSceneBounds(float width, float height);
	Vector2 GetSceneBounds() const;
};