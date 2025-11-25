#pragma once
#include <string>

struct t_WindowConfig 
{
    int width = 1280;
    int height = 720;
    bool b_Fullscreen = false;
    bool b_Resizable = true;
    bool b_Vsync = true;
    int target_fps = 60;
    std::string title = "My Game";
};

class GameConfig 
{
public:
    static GameConfig& GetInstance();
    bool m_bLoadFromFile(const std::string& config_path = "game_config.ini");
    bool m_bSaveToFile(const std::string& config_path = "game_config.ini") const;
    
    t_WindowConfig& GetWindowConfig() { return m_WindowConfig; }
    const t_WindowConfig& GetWindowConfig() const { return m_WindowConfig; }
    
private:
    t_WindowConfig m_WindowConfig;
    GameConfig() = default;
};