#include "Level1.h"
#include <iostream>

Level1::Level1() : GameMap("Cube Game"),  
    m_FinishZone{0.0f, 0.0f, 0.0f, 0.0f},  
    m_PlayerPos{0.0f, 0.0f},  
    m_PlayerVel{0.0f, 0.0f},
    m_bIsGrounded(false)
{
}

void Level1::Initialize()
{
    // Initialize player position to center of screen, slightly above floor
    m_PlayerPos = { 400.0f, FLOOR_Y - PLAYER_SIZE };
    m_PlayerVel = { 0.0f, 0.0f };
    m_bIsGrounded = true;
    
    // Initialize Finish Zone (Invisible Collider at the top)
    m_FinishZone = 
    { 
        0.0f, 
        0.0f, 
        static_cast<float>(GetScreenWidth()), 
        100.0f 
    }; // Top 100 pixels

    // Initialize Obstacles
    m_Obstacles.clear();
    m_Obstacles.emplace_back(600.0f, 400.0f, 100.0f, 20.0f); // Platform 1
    m_Obstacles.emplace_back(200.0f, 350.0f, 100.0f, 20.0f); // Platform 2
    m_Obstacles.emplace_back(400.0f, 250.0f, 100.0f, 20.0f); // Platform 3
    m_Obstacles.emplace_back(800.0f, FLOOR_Y - 60.0f, 60.0f, 60.0f); // Block on floor

    // Initialize Fire Particles
    m_FireParticles.clear();
    m_FireParticles.emplace_back(80.0f, FLOOR_Y - 15.0f);
    m_FireParticles.emplace_back(1200.0f, FLOOR_Y - 15.0f);

    std::cout << "[Level1] Cube Game Initialized" << std::endl;
}

void Level1::Update(float delta_time)
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
    if (m_PlayerPos.x > GetScreenWidth() - PLAYER_SIZE) 
        m_PlayerPos.x = static_cast<float>(GetScreenWidth()) - PLAYER_SIZE;

    // X-Axis Collision
    Rectangle playerRectX = 
    { 
        m_PlayerPos.x, 
        m_PlayerPos.y, 
        PLAYER_SIZE, 
        PLAYER_SIZE 
    };
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
    if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)) && m_bIsGrounded)
    {
        m_PlayerVel.y = JUMP_FORCE;
        m_bIsGrounded = false;
    }

    // Apply Gravity
    m_PlayerVel.y += GRAVITY * delta_time;

    // Apply Velocity
    m_PlayerPos.y += m_PlayerVel.y * delta_time;

    // Y-Axis Collision
    Rectangle playerRectY = 
    { 
        m_PlayerPos.x, 
        m_PlayerPos.y, 
        PLAYER_SIZE, 
        PLAYER_SIZE 
    };
    for (const auto& obstacle : m_Obstacles)
    {
        if (CheckCollisionRecs(playerRectY, obstacle))
        {
            if (m_PlayerVel.y > 0) // Falling down
            {
                m_PlayerPos.y = obstacle.y - PLAYER_SIZE;
                m_PlayerVel.y = 0.0f;
                m_bIsGrounded = true;
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
        m_bIsGrounded = true;
    }

    // Screen Bounds Clamping (Y - Top)
    if (m_PlayerPos.y < 0)
    {
        m_PlayerPos.y = 0;
        m_PlayerVel.y = 0;
    }

    // Win Condition & Level Transition
    // Check collision with Finish Zone
    Rectangle playerRect = 
    { 
        m_PlayerPos.x,
        m_PlayerPos.y, 
        PLAYER_SIZE, 
        PLAYER_SIZE
    };
    if (CheckCollisionRecs(playerRect, m_FinishZone))
    {
        std::cout << "Level Complete! Switching to Level 2..." 
                  << std::endl;
        // Request transition via injected callback
        RequestGotoMap("Level2");
        return; // Stop updating this map
    }

    // Update Fire Particles
    for (auto& particle : m_FireParticles)
    {
        particle.Update(delta_time);
    }
}

void Level1::Draw()
{
    ClearBackground(GRAY);

    // Draw Floor
    DrawRectangle
    (
        0, 
        static_cast<int>(FLOOR_Y), 
        GetScreenWidth(), 
        GetScreenHeight() - static_cast<int>(FLOOR_Y), 
        Color{ 54, 54, 54, 255 }
    );

    // Draw Obstacles
    for (const auto& obstacle : m_Obstacles)
    {
        DrawRectangleRec(obstacle, DARKGRAY);
    }

    // Draw Fire Particles
    for (auto& particle : m_FireParticles)
    {
        particle.Draw();
    }

    // Draw Finish Zone
    DrawRectangleRec(m_FinishZone, Color{ 255, 0, 0, 128 }); // Semi-transparent red

    // Draw Player (Red Cube)
    DrawRectangle
    (
        static_cast<int>(m_PlayerPos.x), 
        static_cast<int>(m_PlayerPos.y), 
        static_cast<int>(PLAYER_SIZE), 
        static_cast<int>(PLAYER_SIZE), 
        RED
    );
}
