#pragma once

#include <chrono>
#include <filesystem>
#include <system_error>

#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>
#include <thread>
#include <tinyfiledialogs.h>

#include "DllLoader.h"
#include "GameEditorLayout.h"
#include "GameEditorTheme.h"
#include "GameEngine.h"
namespace fs = std::filesystem;

class GameEditor 
{
public:
    GameEditor();
    ~GameEditor();

    bool b_IsPlaying;
    bool b_IsCompiling;
    void Init(int width, int height, const char* title);
    void LoadMap(std::unique_ptr<GameMap> &game_map);

    // Load the game logic DLL and create/set a new GameMap from it
    bool b_LoadGameLogic(const char* dllPath);

    // Unload and reload the DLL, then recreate the GameMap
    bool b_ReloadGameLogic();

	void Run();
private:
    void Close() const;

	GameEngine m_GameEngine;
    ImGuiViewport* m_Viewport;

    RenderTexture2D m_RaylibTexture;
    RenderTexture2D m_DisplayTexture;

	void DrawSceneWindow();
    void DrawMapSelectionUI();

    // New icon texture members
    Texture2D m_PlayIcon;
    Texture2D m_PauseIcon;
	Texture2D m_RestartIcon;
    Texture2D m_RestoreIcon;

    bool m_bIconsLoaded;
    void LoadIconTextures();
    void DrawToolbarBackground();

    // Hot-reload state
    DllHandle m_GameLogicDll;
    using CreateGameMapFunc = GameMap * (*)();
    CreateGameMapFunc m_CreateGameMap = nullptr;

    std::string m_GameLogicPath;
    std::filesystem::file_time_type m_LastLogicWriteTime{};

    float m_ReloadCheckAccum = 0.0f;
    Shader m_OpaqueShader;
    bool m_bUseOpaquePass = true;
    
    // Map selection UI
    MapManager* m_MapManager = nullptr;
    std::string m_SelectedMapId;
};