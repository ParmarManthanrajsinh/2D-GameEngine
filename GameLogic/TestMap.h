#pragma once
#include "../Engine/GameMap.h"
//#include <iostream>

class TestMap : public GameMap
{
private:
    Vector2 m_Position;        
    Vector2 m_Velocity;        
    Vector2 m_SceneDimension;  
    float m_Radius;            
    float m_Speed;             
    short m_Direction;         

public:
    TestMap();
    ~TestMap() override;
    
    void Initialize() override;
    void Update(float delta_time) override;  
    void Draw() override;
};