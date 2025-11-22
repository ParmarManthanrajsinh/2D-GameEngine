#pragma once
#include "../Engine/GameMap.h"
#include "FireParticle.h"
#include <raylib.h>
#include <vector>

class Level2 : public GameMap
{
private:
    // Player properties
    Vector2 m_PlayerPos;
    Vector2 m_PlayerVel;
    bool m_bIsGrounded;

    // Obstacles
    std::vector<Rectangle> m_Obstacles;
    
    // Fire Particles
    std::vector<FireParticle> m_FireParticles;

    // Constants
    const float PLAYER_SIZE = 50.0f;
    const float GRAVITY = 1000.0f;
    const float JUMP_FORCE = -600.0f;
    const float MOVE_SPEED = 300.0f;
    const float FLOOR_Y = 500.0f;

public:
    Level2();
    ~Level2() override = default;

    void Initialize() override;
    void Update(float delta_time) override;
    void Draw() override;
};
