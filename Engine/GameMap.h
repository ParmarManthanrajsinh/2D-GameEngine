#include <iostream>
#include <string>

class GameMap
{
protected:
    std::string m_MapName;
    float m_SceneWidth = 0.0f;   // Initialized
    float m_SceneHeight = 0.0f;  // Initialized

public:
    GameMap() : m_MapName("DefaultMap"){}

    GameMap(const std::string& map_name)
        : m_MapName(map_name)
    {
    }

    virtual ~GameMap() = default;  // Virtual to safely delete derived classes

    virtual void Initialize()
    {
        // Initialization code for the game map
    }

    virtual void Update(float DeltaTime)
    {
        // Update logic for the game map
    }

    virtual void Draw()
    {
        // Drawing logic for the game map
    }

    void SetMapName(const std::string& map_name)
    {
        m_MapName = map_name;
    }

    std::string GetMapName() const
    {
        return m_MapName;
    }

    void SetSceneBounds(float width, float height)
    {
        m_SceneWidth = width;
        m_SceneHeight = height;
    }
};
