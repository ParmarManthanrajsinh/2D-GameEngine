#include "GameLogic.h"

GameLogic::GameLogic(CoreEngine *core_engine)
    : m_CoreEngine(core_engine)
{
}

void GameLogic::Init()
{
    // Initialize game state
    m_player.position = {400, 300};
    m_score = 0;
    bm_GameStarted = true;
    m_EnemySpawnTimer = 0.0f;

    // Clear any existing entities
    m_enemies.clear();
    m_bullets.clear();

    // Spawn initial enemies
    for (int i = 0; i < 3; i++)
    {
        t_Enemy enemy;
        enemy.position = {static_cast<float>(100 + i * 200), static_cast<float>(100 + i * 50)};
        enemy.velocity = {static_cast<float>((i % 2 == 0) ? 50 : -50), static_cast<float>(30 + i * 10)};
        m_enemies.push_back(enemy);
    }
}

void GameLogic::Update(float deltaTime)
{
    if (!bm_GameStarted || !m_CoreEngine)
    {
        return;
    }
    // Handle player input (WASD movement)
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        m_player.position.y -= m_player.speed * deltaTime;
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    {
        m_player.position.y += m_player.speed * deltaTime;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        m_player.position.x -= m_player.speed * deltaTime;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        m_player.position.x += m_player.speed * deltaTime;
    }

    // Shoot bullets with SPACE
    if (IsKeyPressed(KEY_SPACE))
    {
        t_Bullet bullet;
        bullet.position = m_player.position;
        bullet.velocity = {0, -300}; // Shoot upward
        m_bullets.push_back(bullet);
    }

    // Keep player in scene bounds
    if (m_player.position.x < m_player.radius)
        m_player.position.x = m_player.radius;
    if (m_player.position.x > m_SceneWidth - m_player.radius)
        m_player.position.x = m_SceneWidth - m_player.radius;
    if (m_player.position.y < m_player.radius)
        m_player.position.y = m_player.radius;
    if (m_player.position.y > m_SceneHeight - m_player.radius)
        m_player.position.y = m_SceneHeight - m_player.radius;

    // Update bullets
    for (auto &bullet : m_bullets)
    {
        if (bullet.active)
        {
            bullet.position.x += bullet.velocity.x * deltaTime;
            bullet.position.y += bullet.velocity.y * deltaTime;

            // Remove bullets that are off screen
            if (bullet.position.y < 0 || bullet.position.y > m_SceneHeight ||
                bullet.position.x < 0 || bullet.position.x > m_SceneWidth)
            {
                bullet.active = false;
            }
        }
    }

    // Update enemies
    for (auto &enemy : m_enemies)
    {
        if (enemy.active)
        {
            enemy.position.x += enemy.velocity.x * deltaTime;
            enemy.position.y += enemy.velocity.y * deltaTime;

            // Bounce off walls
            if (enemy.position.x <= enemy.radius || enemy.position.x >= m_SceneWidth - enemy.radius)
            {
                enemy.velocity.x *= -1;
            }
            if (enemy.position.y <= enemy.radius || enemy.position.y >= m_SceneHeight - enemy.radius)
            {
                enemy.velocity.y *= -1;
            }
        }
    }

    // Check bullet-enemy collisions
    for (auto &bullet : m_bullets)
    {
        if (!bullet.active)
        {
            continue;
        }
        for (auto &enemy : m_enemies)
        {
            if (!enemy.active)
            {
                continue;
            }
            if (m_CoreEngine->CheckCollisionCircles(bullet.position, bullet.radius, enemy.position, enemy.radius))
            {
                bullet.active = false;
                enemy.active = false;
                m_score += 10;
            }
        }
    }

    // Spawn new enemies periodically
    m_EnemySpawnTimer += deltaTime;
    if (m_EnemySpawnTimer > 3.0f)
    {
        t_Enemy enemy;
        enemy.position = {static_cast<float>(rand() % static_cast<int>(m_SceneWidth - 100) + 50), 50};
        enemy.velocity = {static_cast<float>(rand() % 100 - 50), static_cast<float>(rand() % 50 + 50)};
        m_enemies.push_back(enemy);
        m_EnemySpawnTimer = 0.0f;
    }

    // Remove inactive entities
    m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(),
                                   [](const t_Bullet &b)
                                   { return !b.active; }),
                    m_bullets.end());
    m_enemies.erase(std::remove_if(m_enemies.begin(), m_enemies.end(),
                                   [](const t_Enemy &e)
                                   { return !e.active; }),
                    m_enemies.end());
}

void GameLogic::Render()
{
    if (!m_CoreEngine)
    {
        return;
    }
    // Render background
    m_CoreEngine->DrawRectangle(0, 0, static_cast<int>(m_SceneWidth), static_cast<int>(m_SceneHeight), DARKBLUE);

    // Render player
    m_CoreEngine->DrawCircle(static_cast<int>(m_player.position.x), static_cast<int>(m_player.position.y),
                             m_player.radius, m_player.color);

    // Render bullets
    for (const auto &bullet : m_bullets)
    {
        if (bullet.active)
        {
            m_CoreEngine->DrawCircle
            (
                static_cast<int>(bullet.position.x), 
                static_cast<int>(bullet.position.y),
                bullet.radius, 
                bullet.color
            );
        }
    }

    // Render enemies
    for (const auto &enemy : m_enemies)
    {
        if (enemy.active)
        {
            m_CoreEngine->DrawCircle
            (
                static_cast<int>(enemy.position.x), 
                static_cast<int>(enemy.position.y),
                enemy.radius, 
                enemy.color
            );
        }
    }

    // Render UI
    m_CoreEngine->DrawText("SPACE SHOOTER DEMO", 10, 10, 20, WHITE);
    m_CoreEngine->DrawText(("Score: " + std::to_string(m_score)).c_str(), 10, 40, 16, WHITE);
    m_CoreEngine->DrawText("WASD: Move, SPACE: Shoot", 10, 60, 14, LIGHTGRAY);

    // Show entity counts
    m_CoreEngine->DrawText(("Enemies: " + std::to_string(m_enemies.size())).c_str(), 10, 80, 14, LIGHTGRAY);
    m_CoreEngine->DrawText(("Bullets: " + std::to_string(m_bullets.size())).c_str(), 10, 100, 14, LIGHTGRAY);
}
