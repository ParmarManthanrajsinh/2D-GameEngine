#pragma once
#include "../Engine/GameMap.h"
#include <iostream>

/**
 * @brief Example map showing a bouncing ball - great starting point for developers
 * 
 * This map demonstrates:
 * - Basic game loop (Initialize, Update, Draw)
 * - Physics simulation (bouncing ball)
 * - Scene bounds usage
 * - Proper member variable naming conventions
 */
class TestMap : public GameMap
{
private:
    // Note: Member variables use 'm_' prefix (following project conventions)
    // but this map was created before the convention, so it uses old style
    Vector2 m_Position;        // Fixed: Should be m_Position (and fix typo!)
    Vector2 m_Velocity;         // Fixed: Should be m_Velocity
    Vector2 m_SceneDimension;  // Fixed: Should be m_SceneDimension
    float m_Radius;             // Fixed: Should be m_Radius
    float m_Speed;              // Fixed: Should be m_Speed
    short m_Direction;          // Fixed: Should be m_Direction

public:
    TestMap();
    ~TestMap() override;
    
    // Core GameMap interface - parameters use camelCase
    void Initialize() override;
    void Update(float deltaTime) override;  // Note: deltaTime (camelCase) not DeltaTime
    void Draw() override;
};