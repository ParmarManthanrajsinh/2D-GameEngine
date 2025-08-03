#pragma once

#include <vector>
#include <raylib.h>
#include <algorithm>
#include <cmath>
#include "CoreEngine.h"

class CoreEngine;
/**
 * GameLogic - This is where game developers write their game code
 *
 * Game developers should:
 * 1. Override Init() to initialize their game
 * 2. Override Update(float deltaTime) to handle game logic and input
 * 3. Override Render() to draw their game (without BeginFrame/EndFrame)
 * 4. Use GetCoreEngine() to access rendering and input functions
 */
class GameLogic
{
    bool bm_IsRunning = false;
    
public:
    bool IsRunning() const { return bm_IsRunning; }
    void SetRunning(bool running) { bm_IsRunning = running; }

    GameLogic(CoreEngine *core_engine);
    virtual ~GameLogic() = default;

    // Called once when the game starts
    virtual void Init();

    // Called every frame - handle input and update game state
    virtual void Update(float deltaTime);

    // Called every frame - render the game world (called within render context)
    virtual void Render();

    // Get core engine for advanced usage
    CoreEngine *GetCoreEngine() const 
    { 
        return m_CoreEngine; 
    }

protected:
    CoreEngine *m_CoreEngine;

    // Example game entities - developers can replace with their own
    struct t_Player
    {
        Vector2 position = {400, 300};
        float speed = 200.0f;
        float radius = 20.0f;
        Color color = BLUE;
    };

    struct t_Enemy
    {
        Vector2 position = {0, 0};
        Vector2 velocity = {0, 0};
        float radius = 15.0f;
        Color color = RED;
        bool active = true;
    };

    struct t_Bullet
    {
        Vector2 position = {0, 0};
        Vector2 velocity = {0, 0};
        float radius = 3.0f;
        Color color = YELLOW;
        bool active = true;
    };

    // Example game state
    t_Player m_player;
    std::vector<t_Enemy> m_enemies;
    std::vector<t_Bullet> m_bullets;
    int m_score = 0;
    bool bm_GameStarted = false;
    float m_EnemySpawnTimer = 0.0f;

    // Scene bounds (set by the SceneViewPanel)
    float m_SceneWidth = 800.0f;
    float m_SceneHeight = 600.0f;

public:
    // Method to set scene bounds from the editor
    void SetSceneBounds(float width, float height)
    {
        m_SceneWidth = width;
        m_SceneHeight = height;
    }
};