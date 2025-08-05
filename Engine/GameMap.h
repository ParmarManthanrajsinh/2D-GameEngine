#include <iostream>
#include <string>

class GameMap
{
protected:
    std::string m_map_name;
    float m_scene_width = 0.0f;   // Initialized
    float m_scene_height = 0.0f;  // Initialized

public:
    GameMap() : m_map_name("DefaultMap") {}

    GameMap(const std::string& map_name)
        : m_map_name(map_name)
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
        m_map_name = map_name;
    }

    std::string GetMapName() const
    {
        return m_map_name;
    }

    void SetSceneBounds(float width, float height)
    {
        m_scene_width = width;
        m_scene_height = height;
    }
};
