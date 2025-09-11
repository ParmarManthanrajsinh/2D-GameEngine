#include "GameEditor.h"
#include "../Game/DllLoader.h"
#include "../Engine/MapManager.h"
using Clock = std::chrono::steady_clock;

GameEditor::GameEditor()
	: m_Viewport(nullptr),
	  m_RaylibTexture({ 0 }),
	  m_DisplayTexture({ 0 }),
	  b_IsPlaying(false),
	  m_PlayIcon({ 0 }),
	  m_PauseIcon({ 0 }),
	  m_RestartIcon({ 0 }),
	  m_folder_texture ({0}),
	  m_file_texture({0}),
	  m_image_texture({0}),
	  m_text_texture({0}),
	  m_bIconsLoaded(false),
	  m_GameLogicDll{},
	  m_CreateGameMap(nullptr),
	  m_OpaqueShader({ 0 })
{
}

GameEditor::~GameEditor()
{
	/* 
		Ensure any GameMap instance(potentially from the DLL) is destroyed
		BEFORE unloading the DLL, otherwise vtable/function code may be gone
		when the map's destructor runs.
	*/
	m_GameEngine.SetMap(nullptr);
	m_GameEngine.SetMapManager(nullptr); // Also clear the MapManager

	if (m_GameLogicDll.handle)
	{
		UnloadDll(m_GameLogicDll);
		m_GameLogicDll = {};
		m_CreateGameMap = nullptr;
	}
}

void GameEditor::Init(int width, int height, const char* title)
{
	m_GameEngine.LaunchWindow(width, height, title);
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	rlImGuiSetup(true);

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	rlImGuiReloadFonts();

	SetEngineTheme();
	LoadEditorDefaultIni();

	SetTargetFPS(60);
	m_Viewport = ImGui::GetMainViewport();

	m_RaylibTexture = LoadRenderTexture(1280, 720);
	m_DisplayTexture = LoadRenderTexture(1280, 720);

	SetTextureFilter(m_RaylibTexture.texture, TEXTURE_FILTER_BILINEAR);
	SetTextureFilter(m_DisplayTexture.texture, TEXTURE_FILTER_BILINEAR);

	m_OpaqueShader = LoadShader
	(
		"Assets/Shaders/Opaque.vert", 
		"Assets/Shaders/Opaque.frag"
	);

	// Load icon textures
	LoadIconTextures();
}

void GameEditor::LoadIconTextures()
{
	// Load PNG icons (replace paths with your actual icon files)
	Image play_img = LoadImage("Assets/icons/play.png");
	Image pause_img = LoadImage("Assets/icons/pause.png");
	Image restart_img = LoadImage("Assets/icons/restart.png");
	Image folder_img = LoadImage("Assets/icons/folder.png");
	Image file_img = LoadImage("Assets/icons/file.png");
	Image image_img = LoadImage("Assets/icons/image.png");
	Image text_img = LoadImage("Assets/icons/text.png");

	// Convert to textures
	if (folder_img.data) 
	{
		m_folder_texture = LoadTextureFromImage(folder_img);
		UnloadImage(folder_img);
	}

	if (file_img.data) 
	{
		m_file_texture = LoadTextureFromImage(file_img);
		UnloadImage(file_img);
	}

	if (image_img.data) 
	{
		m_image_texture = LoadTextureFromImage(image_img);
		UnloadImage(image_img);
	}

	if (text_img.data) 
	{
		m_text_texture = LoadTextureFromImage(text_img);
		UnloadImage(text_img);
	}

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
		static auto s_LastReloadCheckTime = Clock::now();

		// Periodically check for GameLogic.dll changes (e.g., every 0.5s)
		auto current_time = Clock::now();
		auto elapsed_time = std::chrono::duration<float>
		(
			current_time - s_LastReloadCheckTime
		).count();

		if (elapsed_time > 0.5f && !m_GameLogicPath.empty())
		{
			s_LastReloadCheckTime = current_time;
			std::error_code ec;

			// cache path once per check
			const std::filesystem::path PATH(m_GameLogicPath);
			auto now_write = std::filesystem::last_write_time(PATH, ec);

			if (!ec)
			{
				if (m_LastLogicWriteTime != std::filesystem::file_time_type{})
				{
					if (now_write != m_LastLogicWriteTime)
					{
						b_ReloadGameLogic();
						m_LastLogicWriteTime = now_write;  // update after reload
					}
				}
				else
				{
					m_LastLogicWriteTime = now_write;
				}
			}
		}

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

		Texture2D source_tex = m_RaylibTexture.texture;

		// Opaque pass to strip alpha before presenting via ImGui
		if (m_bUseOpaquePass)
		{
			BeginTextureMode(m_DisplayTexture);
			ClearBackground(BLANK);
			BeginShaderMode(m_OpaqueShader);
			Rectangle src = { 0, 0, (float)source_tex.width, -(float)source_tex.height };
			DrawTextureRec(source_tex, src, { 0.0f, 0.0f }, WHITE);
			EndShaderMode();
			EndTextureMode();
			source_tex = m_DisplayTexture.texture;
		}

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

	if (m_DisplayTexture.id != 0)
	{
		UnloadRenderTexture(m_DisplayTexture);
	}

	rlImGuiShutdown();
	CloseWindow();
}

void GameEditor::DrawExploreWindow()
{
	ImGui::Begin("File Explorer");

	ImGui::Text("Project Assets");
	ImGui::Separator();
	ImGui::Spacing();

	fs::path assets_path = "Assets";
	if (fs::exists(assets_path) && fs::is_directory(assets_path))
	{
		DrawDirectoryTree(assets_path);
	}
	else
	{
		ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.6f, 1.0f), "Assets directory not found.");
	}
	ImGui::End();
}

void GameEditor::DrawDirectoryTree(const fs::path& directory_path)
{
	try
	{
		for (const auto& entry : fs::directory_iterator(directory_path))
		{
			std::string filename = entry.path().filename().string();

			if (entry.is_directory())
			{
				// Draw folder icon if available
				if (m_folder_texture.id != 0)
				{
					ImGui::Image((void*)(intptr_t)m_folder_texture.id, ImVec2(16, 16));
					ImGui::SameLine(0.0f, 4.0f); // Small spacing
				}

				// Use TreeNodeEx for better control
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
				bool node_open = ImGui::TreeNodeEx(filename.c_str(), flags);

				if (node_open)
				{
					DrawDirectoryTree(entry.path());
					ImGui::TreePop();
				}
			}
			else if (Texture2D* icon_texture = nullptr; // For declaration
				entry.is_regular_file())
			{
				// Get file extension
				std::string extension = entry.path().extension().string();

				// Choose appropriate icon and color

				if 
				(	
					extension == ".png"  || 
					extension == ".jpg"  || 
					extension == ".jpeg" ||
					extension == ".bmp"  || 
					extension == ".tga"  || 
					extension == ".gif"
				)
				{
					icon_texture = &m_image_texture;
				}
				else if
				(
					extension == ".txt" || 
					extension == ".md"	|| 
					extension == ".log"
				)
				{
					icon_texture = &m_text_texture;
				}
				else
				{
					icon_texture = &m_file_texture;
				}

				// Draw icon if available
				if (icon_texture != nullptr && icon_texture->id != 0)
				{
					ImGui::Image((void*)(intptr_t)icon_texture->id, ImVec2(16, 16));
					ImGui::SameLine(0.0f, 2.0f); // Reduce spacing to 2 pixels
				}

				ImGui::Text(filename.c_str()); // Changed from BulletText to Text
			}
		}
	}
	catch (const fs::filesystem_error& ex)
	{
		ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "Error reading directory: %s", ex.what());
	}
}

void GameEditor::DrawToolbarBackground()
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
		(
			ImGui::ImageButton
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
		) || IsWindowResized()
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

	if (m_bUseOpaquePass)
	{
		rlImGuiImageRenderTextureFit(&m_DisplayTexture, true);
	}
	else
	{
		rlImGuiImageRenderTextureFit(&m_RaylibTexture, true);
	}

	ImGui::End();
}

void GameEditor::LoadMap(std::unique_ptr<GameMap>& game_map)
{
    if (game_map)
    {
        // Check if the loaded map is a MapManager
        MapManager* mapManager = dynamic_cast<MapManager*>(game_map.get());
        if (mapManager)
        {
            // If it's a MapManager, set it using the dedicated method
            std::unique_ptr<MapManager> ownedMapManager(static_cast<MapManager*>(game_map.release()));
            m_GameEngine.SetMapManager(std::move(ownedMapManager));
        }
        else
        {
            // Otherwise, use the regular SetMap method
            m_GameEngine.SetMap(std::move(game_map));
        }
    }
    else
    {
        m_GameEngine.SetMap(std::make_unique<GameMap>());
    }
}

bool GameEditor::b_LoadGameLogic(const char* dll_path)
{
	m_GameLogicPath = dll_path ? dll_path : "";

	// 1) Load new DLL
	DllHandle new_dll = LoadDll(dll_path);
	if (!new_dll.handle)
	{
		std::cerr << "Failed to load GameLogic DLL: "
			<< dll_path
			<< std::endl;

		return false;
	}

	// 2) Get factory
	CreateGameMapFunc NewFactory =
	reinterpret_cast<CreateGameMapFunc>
	(
		GetDllSymbol(new_dll, "CreateGameMap")
	);

	if (!NewFactory)
	{
		std::cerr << "Failed to get CreateGameMap from DLL" << std::endl;
		UnloadDll(new_dll);
		return false;
	}

	// 3) Create the new map before disturbing current state
	std::unique_ptr<GameMap> new_map(NewFactory());
	if (!new_map)
	{
		std::cerr << "CreateGameMap returned null" << std::endl;
		UnloadDll(new_dll);
		return false;
	}

	// 4) Destroy current map to release old DLL code before unloading
	m_GameEngine.SetMap(nullptr);
	m_GameEngine.SetMapManager(nullptr);

	// 5) Unload old DLL (if any)
	if (m_GameLogicDll.handle)
	{
		UnloadDll(m_GameLogicDll);
		m_GameLogicDll = {};
		m_CreateGameMap = nullptr;
	}

	// 6) Swap in new DLL and map
	m_GameLogicDll = new_dll;
	m_CreateGameMap = NewFactory;
	
	// Check if the loaded map is a MapManager
	MapManager* mapManager = dynamic_cast<MapManager*>(new_map.get());
	if (mapManager)
	{
		// If it's a MapManager, set it using the dedicated method
		std::unique_ptr<MapManager> ownedMapManager(static_cast<MapManager*>(new_map.release()));
		m_GameEngine.SetMapManager(std::move(ownedMapManager));
	}
	else
	{
		// Otherwise, use the regular SetMap method
		m_GameEngine.SetMap(std::move(new_map));
	}

	// Update watched timestamp 
	// (watch the original DLL path, not the shadow)
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
	if (m_GameLogicPath.empty())
	{
		return false;
	}

	bool b_WasPlaying = b_IsPlaying;
	b_IsPlaying = false;

	bool b_Ok = b_LoadGameLogic(m_GameLogicPath.c_str());
	b_IsPlaying = b_WasPlaying;

	return b_Ok;
}