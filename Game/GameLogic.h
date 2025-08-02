#pragma once

#include <vector>
#include <raylib.h>

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
public:
    GameLogic(CoreEngine *coreEngine);
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
        return m_coreEngine; 
    }

protected:
    CoreEngine *m_coreEngine;

    // Example game entities - developers can replace with their own
    struct Player
    {
        Vector2 position = {400, 300};
        float speed = 200.0f;
        float radius = 20.0f;
        Color color = BLUE;
    };

    struct Enemy
    {
        Vector2 position = {0, 0};
        Vector2 velocity = {0, 0};
        float radius = 15.0f;
        Color color = RED;
        bool active = true;
    };

    struct Bullet
    {
        Vector2 position = {0, 0};
        Vector2 velocity = {0, 0};
        float radius = 3.0f;
        Color color = YELLOW;
        bool active = true;
    };

    // Example game state
    Player m_player;
    std::vector<Enemy> m_enemies;
    std::vector<Bullet> m_bullets;
    int m_score = 0;
    bool m_gameStarted = false;
    float m_enemySpawnTimer = 0.0f;

    // Scene bounds (set by the SceneViewPanel)
    float m_sceneWidth = 800.0f;
    float m_sceneHeight = 600.0f;

public:
    // Method to set scene bounds from the editor
    void SetSceneBounds(float width, float height)
    {
        m_sceneWidth = width;
        m_sceneHeight = height;
    }
};
