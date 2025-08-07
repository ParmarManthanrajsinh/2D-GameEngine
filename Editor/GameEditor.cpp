#include "GameEditor.h"

GameEditor::GameEditor()
	: m_viewport(nullptr),
	  m_RaylibTexture({ 0 }),
	  m_LastSize({ 1280, 720 }),
	  b_IsPlaying(false),
	  m_PlayIcon({ 0 }),
	  m_PauseIcon({ 0 }),
	  m_RestartIcon({ 0 }),
	  m_IconsLoaded(false)
{
}

GameEditor::~GameEditor()
{
	// Cleanup icon textures
	if (m_IconsLoaded)
	{
		UnloadTexture(m_PlayIcon);
		UnloadTexture(m_PauseIcon);
		UnloadTexture(m_RestartIcon);
	}
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

	// Load icon textures
	LoadIconTextures();
}

void GameEditor::LoadIconTextures()
{
	// Load PNG icons (replace paths with your actual icon files)
	Image play_img = LoadImage("Assets/icons/play.png");
	Image pause_img = LoadImage("Assets/icons/pause.png");
	Image restart_img = LoadImage("Assets/icons/restart.png");

	// Create fallback icons if files don't exist
	if (play_img.data == nullptr)
	{
		play_img = GenImageColor(20, 20, GREEN);
		// Draw a simple play triangle
		ImageDrawRectangle(&play_img, 6, 4, 2, 12, DARKGREEN);
		ImageDrawRectangle(&play_img, 8, 6, 2, 8, DARKGREEN);
		ImageDrawRectangle(&play_img, 10, 8, 2, 4, DARKGREEN);
	}

	if (pause_img.data == nullptr)
	{
		pause_img = GenImageColor(20, 20, ORANGE);
		// Draw pause bars
		ImageDrawRectangle(&pause_img, 6, 4, 3, 12, MAROON);
		ImageDrawRectangle(&pause_img, 11, 4, 3, 12, MAROON);
	}

	if (restart_img.data == nullptr)
	{
		restart_img = GenImageColor(20, 20, BLUE);
		// Draw a simple restart symbol (circle with arrow)
		ImageDrawCircle(&restart_img, 10, 10, 8, DARKBLUE);
		ImageDrawCircle(&restart_img, 10, 10, 6, BLUE);
		ImageDrawRectangle(&restart_img, 12, 6, 4, 2, DARKBLUE);
		ImageDrawRectangle(&restart_img, 14, 8, 2, 2, DARKBLUE);
	}

	// Resize to consistent size
	ImageResize(&play_img, 20, 20);
	ImageResize(&pause_img, 20, 20);
	ImageResize(&restart_img, 20, 20);

	// Convert to textures
	m_PlayIcon = LoadTextureFromImage(play_img);
	m_PauseIcon = LoadTextureFromImage(pause_img);
	m_RestartIcon = LoadTextureFromImage(restart_img);

	// Cleanup temporary images
	UnloadImage(play_img);
	UnloadImage(pause_img);
	UnloadImage(restart_img);

	m_IconsLoaded = true;
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

void DrawToolbarBackground()
{
    // Get current window's draw list
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Determine position and size of the toolbar
    ImVec2 toolbar_pos = ImGui::GetCursorScreenPos();
    ImVec2 toolbar_size = ImVec2(ImGui::GetContentRegionAvail().x, 32.0f);

    // Define gradient colors
    ImU32 toolbar_color_top = IM_COL32(50, 50, 55, 255);
    ImU32 toolbar_color_bottom = IM_COL32(40, 40, 45, 255);

    // Draw gradient-filled rectangle
    draw_list->AddRectFilledMultiColor
	(
        toolbar_pos,
        ImVec2
		(
			toolbar_pos.x + toolbar_size.x, 
			toolbar_pos.y + toolbar_size.y
		),
        toolbar_color_top,     // top-left
        toolbar_color_top,     // top-right
        toolbar_color_bottom,  // bottom-right
        toolbar_color_bottom   // bottom-left
    );
}

void GameEditor::DrawSceneWindow()
{
	ImGui::Begin("Scene");
	DrawToolbarBackground();

	// Compact button styling
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);

	// Play/Pause button with PNG icon
	if (b_IsPlaying)
	{
		if (ImGui::ImageButton("pause_btn", (ImTextureID)(intptr_t)m_PauseIcon.id, ImVec2(20, 20)))
		{
			b_IsPlaying = false;
		}
	}
	else
	{
		if (ImGui::ImageButton("play_btn", (ImTextureID)(intptr_t)m_PlayIcon.id, ImVec2(20, 20)))
		{
			b_IsPlaying = true;
		}
	}

	ImGui::SameLine();
	if (ImGui::ImageButton("restart_btn", (ImTextureID)(intptr_t)m_RestartIcon.id, ImVec2(20, 20)))
	{
		b_IsPlaying = false;
		m_GameEngine.ResetMap();
	}

	// Status indicator
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 12);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);

	if (b_IsPlaying)
	{
		ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "PLAYING");
	}
	else
	{
		ImGui::TextColored(ImVec4(0.8f, 0.2f, 0.2f, 1.0f), "STOPPED");
	}
	ImGui::PopStyleVar(3);
	
	ImVec2 content_size = ImGui::GetContentRegionAvail();

	// Resize debounce
	static double s_LastResizeTime = 0;
	bool b_NeedsResize = false;

	// Only reallocate texture if user stopped resizing for 0.1 sec
	if (b_NeedsResize && (GetTime() - s_LastResizeTime) > 0.1)
	{
		if (content_size.x > 0 && content_size.y > 0)
		{
			UnloadRenderTexture(m_RaylibTexture);
			m_RaylibTexture = LoadRenderTexture
			(
				static_cast<int>(content_size.x), 
				static_cast<int>(content_size.y)
			);
		}
	}

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