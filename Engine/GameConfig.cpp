#include "GameConfig.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

GameConfig& GameConfig::GetInstance() 
{
    static GameConfig s_Instance;
    return s_Instance;
}

bool GameConfig::m_bLoadFromFile(const std::string& config_path) 
{
    std::ifstream file(config_path);
    if (!file.is_open()) 
    {
        std::cout << "Config file not found: " 
                  << config_path << ". Using defaults." << "\n";
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) 
    {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#' || line[0] == ';') 
            continue;
        
        // Find the equals sign
        size_t equal_pos = line.find('=');
        if (equal_pos == std::string::npos) 
            continue;
        
        std::string key = line.substr(0, equal_pos);
        std::string value = line.substr(equal_pos + 1);
        
        // Trim whitespace
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);
        
        // Parse configuration values
        if (key == "width") 
        {
            m_WindowConfig.width = std::stoi(value);
        } 
        else if (key == "height") 
        {
            m_WindowConfig.height = std::stoi(value);
        } 
        else if (key == "b_Fullscreen") 
        {
            m_WindowConfig.b_Fullscreen = (value == "true" || value == "1");
        } 
        else if (key == "b_Resizable") 
        {
            m_WindowConfig.b_Resizable = (value == "true" || value == "1");
        } 
        else if (key == "b_Vsync") 
        {
            m_WindowConfig.b_Vsync = (value == "true" || value == "1");
        } 
        else if (key == "target_fps") 
        {
            m_WindowConfig.target_fps = std::stoi(value);
        }
        else if (key == "title") 
        {
            m_WindowConfig.title = value;
        }
    }
    
    file.close();
    std::cout << "Loaded configuration from: " << config_path << "\n";
    return true;
}

bool GameConfig::m_bSaveToFile(const std::string& config_path) const 
{
    std::ofstream file(config_path);
    if (!file.is_open()) 
    {
        std::cout << "Failed to create config file: " 
                  << config_path << "\n";
        return false;
    }
    
    file << "# Game Configuration File" << "\n";
    file << "# Window Settings" << "\n";
    file << "width=" << m_WindowConfig.width << "\n";
    file << "height=" << m_WindowConfig.height << "\n";
    file << "b_Fullscreen=" 
         << (m_WindowConfig.b_Fullscreen ? "true" : "false") << "\n";
    file << "b_Resizable=" 
         << (m_WindowConfig.b_Resizable ? "true" : "false") << "\n";
    file << "b_Vsync=" 
         << (m_WindowConfig.b_Vsync ? "true" : "false") << "\n";
    file << "target_fps=" << m_WindowConfig.target_fps << "\n";
    file << "title=" << m_WindowConfig.title << "\n";
    
    file.close();
    std::cout << "Saved configuration to: " << config_path << "\n";
    return true;
}