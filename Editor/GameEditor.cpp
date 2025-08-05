#include "GameEditor.h"

GameEditor::GameEditor()
	: m_viewport(nullptr),
	  m_RaylibTexture({ 0 }),
	  m_LastSize({ 1280, 720 }),
	  b_IsPlaying(false),
	  b_Restart(false)
{
}

GameEditor::~GameEditor()
{
}

void GameEditor::Init(int width, int height, std::string title)
{
	m_GameEngine.LaunchWindow(width, height, title);
	rlImGuiSetup(true);

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	rlImGuiReloadFonts();

	SetEngineTheme();
	LoadEditorDefaultIni();

	SetTargetFPS(60);
	m_viewport = ImGui::GetMainViewport();

	m_RaylibTexture = LoadRenderTexture(1280, 720);
	m_LastSize = { 1280, 720 };
}

void GameEditor::Run()
{
	while (!WindowShouldClose())
	{
		float DeltaTime = GetFrameTime();

		if (b_IsPlaying)
		{
			m_GameEngine.UpdateMap(DeltaTime);
		}
		BeginDrawing();

		BeginTextureMode(m_RaylibTexture);
		ClearBackground(RAYWHITE);

		m_GameEngine.DrawMap();
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
	UnloadRenderTexture(m_RaylibTexture);
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
		
	if (ImGui::Button(b_IsPlaying ? "Pause" : "Play"))
	{
		b_IsPlaying = !b_IsPlaying;
	}

	ImGui::SameLine();
	if (ImGui::Button("Restart"))
	{
		b_Restart = true;
	}

	ImVec2 content_size = ImGui::GetContentRegionAvail();

	bool b_is_visible = ImGui::IsWindowAppearing() || ImGui::IsWindowHovered() || ImGui::IsWindowFocused();

	// Resize debounce
	static double s_LastResizeTime = 0;
	bool b_NeedsResize = false;

	if ((int)content_size.x != (int)m_LastSize.x || (int)content_size.y != (int)m_LastSize.y) 
	{
		s_LastResizeTime = GetTime();
		m_LastSize = { content_size.x, content_size.y };
		b_NeedsResize = true;
	}

	// Only reallocate texture if user stopped resizing for 0.1 sec
	if (b_NeedsResize && (GetTime() - s_LastResizeTime) > 0.1) 
	{
		if (content_size.x > 0 && content_size.y > 0) 
		{
			UnloadRenderTexture(m_RaylibTexture);
			m_RaylibTexture = LoadRenderTexture((int)content_size.x, (int)content_size.y);
		}
	}

	//// Skip rendering if window is hidden/collapsed to save GPU
	//if (!ImGui::IsWindowCollapsed() && isVisible && content_size.x > 0 && content_size.y > 0)
	//{
	//	BeginTextureMode(m_RaylibTexture);
	//	ClearBackground(RAYWHITE);
	//	DrawCircle(GetFrameTime() * 100, m_RaylibTexture.texture.height / 2, 50, RED);
	//	DrawText("Raylib Scene in ImGui Window", 10, 10, 20, BLUE);
	//	EndTextureMode();
	//}

	// Draw the texture to ImGui
	ImGui::Image
	(
		(ImTextureID)(intptr_t)m_RaylibTexture.texture.id,
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
		m_GameEngine.SetMap(std::move(game_map));
	}
	else
	{
		m_GameEngine.SetMap(std::make_unique<GameMap>());
	}
}