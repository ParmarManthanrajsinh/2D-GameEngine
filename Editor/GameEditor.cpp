#include "GameEditor.h"
#include "GameEngine.h"

GameEditor::GameEditor()
	: m_viewport(nullptr),
	m_raylib_texture({ 0 }),
	m_last_size({ 1280, 720 })
{
}

GameEditor::~GameEditor()
{
}

void GameEditor::Init(int width, int height, std::string title)
{
	m_game_engine.LaunchWindow(width, height, title);

	rlImGuiSetup(true);

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	rlImGuiReloadFonts();

	SetEngineTheme();
	LoadEditorDefaultIni();

	SetTargetFPS(60);
	m_viewport = ImGui::GetMainViewport();

	m_raylib_texture = LoadRenderTexture(1280, 720);
	m_last_size = { 1280, 720 };
}

void GameEditor::Run()
{
	while (!WindowShouldClose())
	{
		float DeltaTime = GetFrameTime();

		m_game_engine.UpdateMap(DeltaTime);

		BeginDrawing();

		BeginTextureMode(m_raylib_texture);
		ClearBackground(RAYWHITE);

		m_game_engine.DrawMap();

		EndTextureMode();

		rlImGuiBegin();

		ImGui::DockSpaceOverViewport(0, m_viewport);

		DrawExploreWindow();
		DrawSceneWindow();

		rlImGuiEnd();
		EndDrawing();
	}

	Close();
}

void GameEditor::Close() const
{
	UnloadRenderTexture(m_raylib_texture);
	rlImGuiShutdown();
	CloseWindow();
}

void GameEditor::DrawExploreWindow()
{
	ImGui::Begin("File Explorer");
	ImGui::Text("Project Assets");
	ImGui::Separator();
	ImGui::Text("Assets will be displayed here.");
	ImGui::End();
}

void GameEditor::DrawSceneWindow()
{
	ImGui::Begin("Scene");

	ImVec2 content_size = ImGui::GetContentRegionAvail();

	bool b_is_visible = ImGui::IsWindowAppearing() || ImGui::IsWindowHovered() || ImGui::IsWindowFocused();

	// Resize debounce
	static double last_resize_time = 0;
	bool b_needs_resize = false;

	if ((int)content_size.x != (int)m_last_size.x || (int)content_size.y != (int)m_last_size.y) 
	{
		last_resize_time = GetTime();
		m_last_size = { content_size.x, content_size.y };
		b_needs_resize = true;
	}

	// Only reallocate texture if user stopped resizing for 0.1 sec
	if (b_needs_resize && (GetTime() - last_resize_time) > 0.1) 
	{
		if (content_size.x > 0 && content_size.y > 0) 
		{
			UnloadRenderTexture(m_raylib_texture);
			m_raylib_texture = LoadRenderTexture((int)content_size.x, (int)content_size.y);
		}
	}

	//// Skip rendering if window is hidden/collapsed to save GPU
	//if (!ImGui::IsWindowCollapsed() && isVisible && content_size.x > 0 && content_size.y > 0)
	//{
	//	BeginTextureMode(m_raylib_texture);
	//	ClearBackground(RAYWHITE);
	//	DrawCircle(GetFrameTime() * 100, m_raylib_texture.texture.height / 2, 50, RED);
	//	DrawText("Raylib Scene in ImGui Window", 10, 10, 20, BLUE);
	//	EndTextureMode();
	//}

	// Draw the texture to ImGui
	ImGui::Image
	(
		(ImTextureID)(intptr_t)m_raylib_texture.texture.id,
		content_size,
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	ImGui::End();
}

void GameEditor::LoadMap(std::unique_ptr<GameMap>& game_map)
{
	if (game_map)
	{
		m_game_engine.SetMap(std::move(game_map));
	}
	else
	{
		m_game_engine.SetMap(std::make_unique<GameMap>());
	}
}

