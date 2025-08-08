#pragma once

#include <rlImGui.h>
#include <raylib.h>
#include <imgui.h>
#include <tinyfiledialogs.h>
#include <filesystem>

#include "GameEditorTheme.h"
#include "GameEditorLayout.h"
#include "GameEngine.h"
#include "DllLoader.h"

class GameEditor 
{
public:
    GameEditor();
    ~GameEditor();

    bool b_IsPlaying;
    void Init(int width, int height, std::string title);
    void LoadMap(std::unique_ptr<GameMap>& game_map);

    // Load the game logic DLL and create/set a new GameMap from it
    bool LoadGameLogic(const char* dllPath);
    // Unload and reload the DLL, then recreate the GameMap
    bool ReloadGameLogic();
	void Run();
private:
    void Close() const;

	GameEngine m_GameEngine;
    ImGuiViewport* m_Viewport;

    RenderTexture2D m_RaylibTexture;
    Vector2 m_LastSize;

	void DrawExploreWindow();
	void DrawSceneWindow();

    // New icon texture members
    Texture2D m_PlayIcon;
    Texture2D m_PauseIcon;
    Texture2D m_RestartIcon;
    bool m_IconsLoaded;
    void LoadIconTextures();
    friend void DrawToolbarBackground();

    // Hot-reload state
    DllHandle m_GameLogicDll;
    using CreateGameMapFunc = GameMap* (*)();
    CreateGameMapFunc m_CreateGameMap = nullptr;
    std::string m_GameLogicPath;
    std::filesystem::file_time_type m_LastLogicWriteTime{};
    float m_ReloadCheckAccum = 0.0f;
};
