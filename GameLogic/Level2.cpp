#include "Level2.h"
#include <iostream>

Level2::Level2() : GameMap("Level 2")
{
}

void Level2::Initialize()
{
    // Initialize player position
    m_PlayerPos = { 100.0f, FLOOR_Y - PLAYER_SIZE };
    m_PlayerVel = { 0.0f, 0.0f };
    m_IsGrounded = true;
    
    // Initialize Obstacles (Harder layout)
    m_Obstacles.clear();
    m_Obstacles.emplace_back(300.0f, 400.0f, 100.0f, 20.0f);
    m_Obstacles.emplace_back(500.0f, 300.0f, 100.0f, 20.0f);
    m_Obstacles.emplace_back(700.0f, 200.0f, 100.0f, 20.0f);
    m_Obstacles.emplace_back(900.0f, 350.0f, 100.0f, 20.0f); // Tricky jump
    m_Obstacles.emplace_back(1100.0f, 150.0f, 100.0f, 20.0f); // Goal platform

    // Initialize Fire Particles
    m_FireParticles.clear();
    m_FireParticles.emplace_back(50.0f, FLOOR_Y - 15.0f);
    m_FireParticles.emplace_back(1150.0f, 150.0f - 15.0f); // Fire on goal

    std::cout << "[Level2] Initialized" << std::endl;
}

void Level2::Update(float delta_time)
{
    // -------------------------
    // Horizontal Movement (X)
    // -------------------------
    float move_amount = 0.0f;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
        move_amount -= MOVE_SPEED * delta_time;
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        move_amount += MOVE_SPEED * delta_time;
    }

    m_PlayerPos.x += move_amount;

    // Screen Bounds Clamping (X)
    if (m_PlayerPos.x < 0) m_PlayerPos.x = 0;
    if (m_PlayerPos.x > GetScreenWidth() - PLAYER_SIZE) m_PlayerPos.x = (float)GetScreenWidth() - PLAYER_SIZE;

    // X-Axis Collision
    Rectangle playerRectX = { m_PlayerPos.x, m_PlayerPos.y, PLAYER_SIZE, PLAYER_SIZE };
    for (const auto& obstacle : m_Obstacles)
    {
        if (CheckCollisionRecs(playerRectX, obstacle))
        {
            if (move_amount > 0) // Moving Right
            {
                m_PlayerPos.x = obstacle.x - PLAYER_SIZE;
            }
            else if (move_amount < 0) // Moving Left
            {
                m_PlayerPos.x = obstacle.x + obstacle.width;
            }
        }
    }

    // -------------------------
    // Vertical Movement (Y)
    // -------------------------
    
    // Jumping
    if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)) && m_IsGrounded)
    {
        m_PlayerVel.y = JUMP_FORCE;
        m_IsGrounded = false;
    }

    // Apply Gravity
    m_PlayerVel.y += GRAVITY * delta_time;

    // Apply Velocity
    m_PlayerPos.y += m_PlayerVel.y * delta_time;

    // Y-Axis Collision
    Rectangle playerRectY = { m_PlayerPos.x, m_PlayerPos.y, PLAYER_SIZE, PLAYER_SIZE };
    for (const auto& obstacle : m_Obstacles)
    {
        if (CheckCollisionRecs(playerRectY, obstacle))
        {
            if (m_PlayerVel.y > 0) // Falling down
            {
                m_PlayerPos.y = obstacle.y - PLAYER_SIZE;
                m_PlayerVel.y = 0.0f;
                m_IsGrounded = true;
            }
            else if (m_PlayerVel.y < 0) // Jumping up
            {
                m_PlayerPos.y = obstacle.y + obstacle.height;
                m_PlayerVel.y = 0.0f;
            }
        }
    }

    // Floor Collision
    if (m_PlayerPos.y >= FLOOR_Y - PLAYER_SIZE)
    {
        m_PlayerPos.y = FLOOR_Y - PLAYER_SIZE;
        m_PlayerVel.y = 0.0f;
        m_IsGrounded = true;
    }
    
    // Screen Bounds Clamping (Y - Top)
    if (m_PlayerPos.y < 0)
    {
        m_PlayerPos.y = 0;
        m_PlayerVel.y = 0;
    }

    // Update Fire Particles
    for (auto& particle : m_FireParticles)
    {
        particle.Update(delta_time);
    }
}

void Level2::Draw()
{
    ClearBackground(SKYBLUE); // Different background for Level 2

    // Draw Floor
    DrawRectangle(0, (int)FLOOR_Y, GetScreenWidth(), GetScreenHeight() - (int)FLOOR_Y, Color{ 54, 54, 54, 255 });

    // Draw Obstacles
    for (const auto& obstacle : m_Obstacles)
    {
        DrawRectangleRec(obstacle, Color{ 54, 54, 54, 255 });
    }

    // Draw Fire Particles
    for (auto& particle : m_FireParticles)
    {
        particle.Draw();
    }

    // Draw Player (Red Cube)
    DrawRectangle((int)m_PlayerPos.x, (int)m_PlayerPos.y, (int)PLAYER_SIZE, (int)PLAYER_SIZE, RED);
}
