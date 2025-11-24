#include "GameConfig.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

GameConfig& GameConfig::GetInstance() {
    static GameConfig instance;
    return instance;
}

bool GameConfig::LoadFromFile(const std::string& configPath) {
    std::ifstream file(configPath);
    if (!file.is_open()) {
        std::cout << "Config file not found: " << configPath << ". Using defaults." << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#' || line[0] == ';') continue;
        
        // Find the equals sign
        size_t equalPos = line.find('=');
        if (equalPos == std::string::npos) continue;
        
        std::string key = line.substr(0, equalPos);
        std::string value = line.substr(equalPos + 1);
        
        // Trim whitespace
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);
        
        // Parse configuration values
        if (key == "width") {
            m_WindowConfig.width = std::stoi(value);
        } else if (key == "height") {
            m_WindowConfig.height = std::stoi(value);
        } else if (key == "fullscreen") {
            m_WindowConfig.fullscreen = (value == "true" || value == "1");
        } else if (key == "resizable") {
            m_WindowConfig.resizable = (value == "true" || value == "1");
        } else if (key == "vsync") {
            m_WindowConfig.vsync = (value == "true" || value == "1");
        } else if (key == "targetFPS") {
            m_WindowConfig.targetFPS = std::stoi(value);
        } else if (key == "title") {
            m_WindowConfig.title = value;
        }
    }
    
    file.close();
    std::cout << "Loaded configuration from: " << configPath << std::endl;
    return true;
}

bool GameConfig::SaveToFile(const std::string& configPath) {
    std::ofstream file(configPath);
    if (!file.is_open()) {
        std::cerr << "Failed to create config file: " << configPath << std::endl;
        return false;
    }
    
    file << "# Game Configuration File" << std::endl;
    file << "# Window Settings" << std::endl;
    file << "width=" << m_WindowConfig.width << std::endl;
    file << "height=" << m_WindowConfig.height << std::endl;
    file << "fullscreen=" << (m_WindowConfig.fullscreen ? "true" : "false") << std::endl;
    file << "resizable=" << (m_WindowConfig.resizable ? "true" : "false") << std::endl;
    file << "vsync=" << (m_WindowConfig.vsync ? "true" : "false") << std::endl;
    file << "targetFPS=" << m_WindowConfig.targetFPS << std::endl;
    file << "title=" << m_WindowConfig.title << std::endl;
    
    file.close();
    std::cout << "Saved configuration to: " << configPath << std::endl;
    return true;
}