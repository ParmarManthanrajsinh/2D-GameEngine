#include "GameEditor.h"

GameEditor::GameEditor()
	: m_Viewport(nullptr),
	  m_RaylibTexture({ 0 }),
	  m_LastSize({ 1280, 720 }),
	  b_IsPlaying(false),
	  m_PlayIcon({ 0 }),
	  m_PauseIcon({ 0 }),
	  m_RestartIcon({ 0 }),
	  m_bIconsLoaded(false),
	  m_GameLogicDll{},
	  m_CreateGameMap(nullptr)
{
}

GameEditor::~GameEditor()
{
	// Ensure any GameMap instance (potentially from the DLL) is destroyed
	// BEFORE unloading the DLL, otherwise vtable/function code may be gone
	// when the map's destructor runs.
	m_GameEngine.SetMap(nullptr);

	if (m_GameLogicDll.handle)
	{
		UnloadDll(m_GameLogicDll);
		m_GameLogicDll = {};
		m_CreateGameMap = nullptr;
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
	m_Viewport = ImGui::GetMainViewport();

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

	m_bIconsLoaded = true;
}

void GameEditor::Run()
{
	while (!WindowShouldClose())
	{
		float DeltaTime = GetFrameTime();

		// Periodically check for GameLogic.dll changes (e.g., every 0.5s)
		m_ReloadCheckAccum += DeltaTime;
		if (m_ReloadCheckAccum > 0.5f && !m_GameLogicPath.empty())
		{
			m_ReloadCheckAccum = 0.0f;
			std::error_code ec;
			auto now_write = 
			std::filesystem::last_write_time
			(
				std::filesystem::path(m_GameLogicPath), ec
			);

			if
			(	!ec 
				&& m_LastLogicWriteTime != std::filesystem::file_time_type{} 
				&& now_write != m_LastLogicWriteTime
			) { b_ReloadGameLogic(); }
				

			if
			(
				!ec
				&& m_LastLogicWriteTime == std::filesystem::file_time_type{}
			) { m_LastLogicWriteTime = now_write; }
		}

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

		ImGui::DockSpaceOverViewport(0, m_Viewport);

		DrawExploreWindow();
		DrawSceneWindow();

		rlImGuiEnd();
		EndDrawing();
	}

	Close();
}

void GameEditor::Close() const
{
	// Unload GPU resources BEFORE closing the window to keep GL context valid
	if (m_bIconsLoaded)
	{
		UnloadTexture(m_PlayIcon);
		UnloadTexture(m_PauseIcon);
		UnloadTexture(m_RestartIcon);
	}
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
		if 
		(	ImGui::ImageButton
			(
				"pause_btn", 
				(ImTextureID)(intptr_t)m_PauseIcon.id, 
				ImVec2(20, 20)
			)
		)
		{
			b_IsPlaying = false;
		}
	}
	else
	{
		if 
		(
			ImGui::ImageButton
			(
				"play_btn", 
				(ImTextureID)(intptr_t)m_PlayIcon.id, 
				ImVec2(20, 20)
			)
		)
		{
			b_IsPlaying = true;
		}
	}

	ImGui::SameLine();
	if 
	(
		ImGui::ImageButton
		(
			"restart_btn", 
			(ImTextureID)(intptr_t)m_RestartIcon.id, 
			ImVec2(20, 20)
		)
	)
	{
		b_IsPlaying = false;

		// Attempt hot reload of GameLogic.dll, fallback to reset if it fails
		if (!b_ReloadGameLogic())
		{
			m_GameEngine.ResetMap();
		}
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

	// Draw the texture to ImGui
	ImGui::Image
	(
		(ImTextureID)(intptr_t)m_RaylibTexture.texture.id,
		ImGui::GetContentRegionAvail(),
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

bool GameEditor::b_LoadGameLogic(const char* dllPath)
{
	m_GameLogicPath = dllPath ? dllPath : "";

	// 1) Load new DLL
	DllHandle new_dll = LoadDll(dllPath);
	if (!new_dll.handle)
	{
		std::cerr << "Failed to load GameLogic DLL: " << dllPath << std::endl;
		return false;
	}

	// 2) Get factory
	CreateGameMapFunc new_factory = 
	reinterpret_cast<CreateGameMapFunc>
	(
		GetDllSymbol(new_dll, "CreateGameMap")
	);

	if (!new_factory)
	{
		std::cerr << "Failed to get CreateGameMap from DLL" << std::endl;
		UnloadDll(new_dll);
		return false;
	}

	// 3) Create the new map before disturbing current state
	std::unique_ptr<GameMap> new_map(new_factory());
	if (!new_map)
	{
		std::cerr << "CreateGameMap returned null" << std::endl;
		UnloadDll(new_dll);
		return false;
	}

	// 4) Destroy current map to release old DLL code before unloading
	m_GameEngine.SetMap(nullptr);

	// 5) Unload old DLL (if any)
	if (m_GameLogicDll.handle)
	{
		UnloadDll(m_GameLogicDll);
		m_GameLogicDll = {};
		m_CreateGameMap = nullptr;
	}

	// 6) Swap in new DLL and map
	m_GameLogicDll = new_dll;
	m_CreateGameMap = new_factory;
	m_GameEngine.SetMap(std::move(new_map));

	// Update watched timestamp (watch the original DLL path, not the shadow)
	std::error_code ec;

	m_LastLogicWriteTime = 
	std::filesystem::last_write_time
	(
		std::filesystem::path(m_GameLogicPath), 
		ec
	);

	return true;
}

bool GameEditor::b_ReloadGameLogic()
{
	bool b_WasPlaying;
	if (m_GameLogicPath.empty())
	{
		return false;
	}

	b_WasPlaying = b_IsPlaying;
	b_IsPlaying = false;
	bool b_Ok = b_LoadGameLogic(m_GameLogicPath.c_str());
	b_IsPlaying = b_WasPlaying;

	return b_Ok;
}