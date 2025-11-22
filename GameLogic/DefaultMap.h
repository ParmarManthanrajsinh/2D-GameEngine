#pragma once
#include "../Engine/GameMap.h"
#include "../Engine/MapManager.h"
#include "FireParticle.h"
#include <raylib.h>
#include <vector>

class DefaultMap : public GameMap
{
public:
    static MapManager* s_MapManager;

private:
    // Player properties
    Vector2 m_PlayerPos;
    Vector2 m_PlayerVel;
    bool m_bIsGrounded;

    // Finish Zone
    Rectangle m_FinishZone;

    // Obstacles
    std::vector<Rectangle> m_Obstacles;

    // Constants
    const float PLAYER_SIZE = 50.0f;
    const float GRAVITY = 1000.0f;
    const float JUMP_FORCE = -600.0f;
    const float MOVE_SPEED = 300.0f;
    const float FLOOR_Y = 500.0f;
    
    // Fire Particles
    std::vector<FireParticle> m_FireParticles;

public:
    DefaultMap();
    ~DefaultMap() override = default;

    void Initialize() override;
    void Update(float delta_time) override;
    void Draw() override;
};