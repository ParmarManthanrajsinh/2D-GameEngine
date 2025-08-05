#pragma once

#include <rlImGui.h>
#include <raylib.h>
#include <imgui.h>
#include <tinyfiledialogs.h>

#include "GameEditorTheme.h"
#include "GameEditorLayout.h"
#include "GameEngine.h"

class GameEditor 
{
public:
    GameEditor();
    ~GameEditor();

    bool b_IsPlaying;
    bool b_Restart;
    void Init(int width, int height, std::string title);
    void LoadMap(std::unique_ptr<GameMap>& game_map);
	void Run();
private:
    void Close() const;

	GameEngine m_GameEngine;
    ImGuiViewport* m_viewport;

    RenderTexture2D m_RaylibTexture;
    Vector2 m_LastSize;

	void DrawExploreWindow();
	void DrawSceneWindow();
};
