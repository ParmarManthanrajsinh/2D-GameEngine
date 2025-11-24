#pragma once
#include <string>

struct WindowConfig {
    int width = 1280;
    int height = 720;
    bool fullscreen = false;
    bool resizable = true;
    bool vsync = true;
    int targetFPS = 60;
    std::string title = "My Game";
};

class GameConfig {
public:
    static GameConfig& GetInstance();
    bool LoadFromFile(const std::string& configPath = "game_config.ini");
    bool SaveToFile(const std::string& configPath = "game_config.ini");
    
    WindowConfig& GetWindowConfig() { return m_WindowConfig; }
    const WindowConfig& GetWindowConfig() const { return m_WindowConfig; }
    
private:
    WindowConfig m_WindowConfig;
    GameConfig() = default;
};