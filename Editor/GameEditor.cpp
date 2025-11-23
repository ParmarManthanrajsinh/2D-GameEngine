#include "../Engine/MapManager.h"
#include "../Game/DllLoader.h"
#include "GameEditor.h"
using Clock = std::chrono::steady_clock;

GameEditor::GameEditor()
	: m_Viewport(nullptr),
	  m_RaylibTexture({ 0 }),
	  m_DisplayTexture({ 0 }),
	  b_IsPlaying(false),
	  b_IsCompiling(false),
	  m_PlayIcon({ 0 }),
	  m_PauseIcon({ 0 }),
	  m_RestartIcon({ 0 }),
	  m_RestoreIcon({ 0 }),
	  m_CompileIcon({ 0 }),
	  m_bIconsLoaded(false),
	  m_GameLogicDll{},
	  m_CreateGameMap(nullptr),
	  m_OpaqueShader({ 0 }),
	  m_MapManager(nullptr)
{
}

GameEditor::~GameEditor()
{
	/* 
		Ensure any GameMap instance(potentially from the DLL) is destroyed
		BEFORE unloading the DLL, otherwise vtable/function code may be gone
		when the map's destructor runs.
	*/
	m_MapManager = nullptr; 
	m_GameEngine.SetMap(nullptr);
	m_GameEngine.SetMapManager(nullptr); 

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
	Image restore_img = LoadImage("Assets/icons/restore.png");
	Image compile_img = LoadImage("Assets/icons/compile.png");

	// Create fallback icons if files don't exist
	if (play_img.data == nullptr)
	{
		play_img = GenImageColor(28, 28, GREEN); // Updated size

		// Draw a simple play triangle
		ImageDrawRectangle(&play_img, 8, 6, 3, 16, DARKGREEN);
		ImageDrawRectangle(&play_img, 11, 9, 3, 10, DARKGREEN);
		ImageDrawRectangle(&play_img, 14, 12, 3, 4, DARKGREEN);
	}

	if (pause_img.data == nullptr)
	{
		pause_img = GenImageColor(28, 28, ORANGE); // Updated size

		// Draw pause bars
		ImageDrawRectangle(&pause_img, 8, 6, 4, 16, MAROON);
		ImageDrawRectangle(&pause_img, 16, 6, 4, 16, MAROON);
	}

	if (restart_img.data == nullptr)
	{
		restart_img = GenImageColor(28, 28, RED); // Updated size

		// Draw a simple restart symbol (circle with arrow)
		ImageDrawCircle(&restart_img, 14, 14, 10, BLANK);
		ImageDrawCircle(&restart_img, 14, 14, 8, RED);
		ImageDrawRectangle(&restart_img, 16, 8, 6, 3, BLANK);
		ImageDrawRectangle(&restart_img, 18, 11, 3, 3, BLANK);
	}

	if (restore_img.data == nullptr)
	{
		restore_img = GenImageColor(28, 28, BLUE); // Updated size

		// Draw a simple restart symbol (circle with arrow)
		ImageDrawCircle(&restore_img, 14, 14, 10, DARKBLUE);
		ImageDrawCircle(&restore_img, 14, 14, 8, BLUE);
		ImageDrawRectangle(&restore_img, 16, 8, 6, 3, DARKBLUE);
		ImageDrawRectangle(&restore_img, 18, 11, 3, 3, DARKBLUE);
	}

	if (compile_img.data == nullptr)
	{
		compile_img = GenImageColor(28, 28, LIME); // Updated size

		// Draw a simple compile symbol
		ImageDrawRectangle(&compile_img, 10, 6, 6, 16, DARKGREEN);
		ImageDrawRectangle(&compile_img, 6, 10, 14, 6, DARKGREEN);
		ImageDrawRectangle(&compile_img, 8, 8, 10, 10, LIME);
	}

	// Resize to consistent size
	ImageResize(&play_img, 28, 28);
	ImageResize(&pause_img, 28, 28);
	ImageResize(&restart_img, 28, 28);
	ImageResize(&restore_img, 28, 28);
	ImageResize(&compile_img, 28, 28);

	// Convert to textures
	m_PlayIcon = LoadTextureFromImage(play_img);
	m_PauseIcon = LoadTextureFromImage(pause_img);
	m_RestartIcon = LoadTextureFromImage(restart_img);
	m_RestoreIcon = LoadTextureFromImage(restore_img);
	m_CompileIcon = LoadTextureFromImage(compile_img);

	// Cleanup temporary images
	UnloadImage(play_img);
	UnloadImage(pause_img);
	UnloadImage(restart_img);
	UnloadImage(restore_img);
	UnloadImage(compile_img);

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

						// update after reload
						m_LastLogicWriteTime = now_write;  
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
			Rectangle src = 
			{
				0, 
				0, 
				static_cast<float>(source_tex.width), 
				-static_cast<float>(source_tex.height) 
			};
			DrawTextureRec(source_tex, src, { 0.0f, 0.0f }, WHITE);
			EndShaderMode();
			EndTextureMode();
			source_tex = m_DisplayTexture.texture;
		}

		rlImGuiBegin();

		ImGui::DockSpaceOverViewport(0, m_Viewport);

		DrawSceneWindow();
		DrawMapSelectionUI();

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
		UnloadTexture(m_RestoreIcon);
		UnloadTexture(m_CompileIcon);
	}

	UnloadRenderTexture(m_RaylibTexture);

	if (m_DisplayTexture.id != 0)
	{
		UnloadRenderTexture(m_DisplayTexture);
	}

	rlImGuiShutdown();
	CloseWindow();
}

void GameEditor::DrawToolbarBackground()
{
	// Get current window's draw list
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	// Determine position and size of the toolbar
	ImVec2 toolbar_pos = ImGui::GetCursorScreenPos();
	ImVec2 toolbar_size = ImVec2(ImGui::GetContentRegionAvail().x, 40.0f); // Updated from 32.0f to 40.0f

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

// Utility function for build game logic
bool GameEditor::b_StartBuildProcessWithTimeout
(
	const char* command, int timeout_seconds
)
{
	return std::system(command) == 0;
}

static void DrawSpinner(float radius, float thickness, const ImU32& color)
{
	// Group all float variables together for cache locality
	alignas(16) struct {
		float time;
		float start;
		float a_min;
		float a_max;
		float centre_x;
		float centre_y;
		float time_x8;
		float inv_num_segments;
		float angle_range;
	} vars{};

	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImGui::Dummy(ImVec2(radius * 2, radius * 2));

	ImDrawList* DrawList = ImGui::GetWindowDrawList();
	DrawList->PathClear();

	vars.time = static_cast<float>(ImGui::GetTime());
	constexpr int NUM_SEGMENTS = 30;
	vars.start = fabsf(sinf(vars.time * 1.8f) * (NUM_SEGMENTS - 5));

	// Precompute invariant values
	vars.inv_num_segments = 1.0f / static_cast<float>(NUM_SEGMENTS);
	vars.time_x8 = vars.time * 8.0f;

	vars.a_min = 3.14159f * 2.0f * vars.start * vars.inv_num_segments;
	vars.a_max = 3.14159f * 2.0f * static_cast<float>(NUM_SEGMENTS - 3) * vars.inv_num_segments;
	vars.angle_range = vars.a_max - vars.a_min;

	const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius);
	vars.centre_x = centre.x;
	vars.centre_y = centre.y;

	// Pre-allocate path memory
	for (int i = 0; i < NUM_SEGMENTS; i++)
	{
		const float a = vars.a_min + (static_cast<float>(i) * vars.inv_num_segments) * vars.angle_range;

		const float angle = a + vars.time_x8;

		DrawList->PathLineTo
		(
			ImVec2
			(
				vars.centre_x + cosf(angle) * radius,
				vars.centre_y + sinf(angle) * radius
			)
		);
	}

	DrawList->PathStroke(color, false, thickness);
}

void GameEditor::DrawSceneWindow()
{
	ImGui::Begin("Scene");
	DrawToolbarBackground();

	// Compact button styling
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);

	// Calculate vertical centering for toolbar (40px tall)
	// Button with padding = 28 + 4*2 = 36px
	float toolbar_height = 40.0f;
	float button_height = 28.0f + ImGui::GetStyle().FramePadding.y * 2.0f;
	float vertical_offset = (toolbar_height - button_height) / 2.0f;
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + vertical_offset);

	// Play/Pause button with PNG icon
	if (b_IsPlaying)
	{
		if
		(
			ImGui::ImageButton
			(
				"pause_btn",
				(ImTextureID)(intptr_t)m_PauseIcon.id,
				ImVec2(28, 28)
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
				ImVec2(28, 28)
			)
		)
		{
			b_IsPlaying = true;
		}
	}

	// Restart button with PNG icon
	ImGui::SameLine();
	if
	(
		ImGui::ImageButton
		(
			"restart_btn",
			(ImTextureID)(intptr_t)m_RestartIcon.id,
			ImVec2(28, 28)
		) || IsWindowResized()
	)
	{
		b_IsPlaying = false;
		m_MapManager->b_ReloadCurrentMap();
	}

	// Status indicator
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 12);
	// Center text vertically in toolbar
	float text_y_offset = 
	(
		toolbar_height - ImGui::GetTextLineHeight()
	) / 2.0f;

	ImGui::SetCursorPosY
	(
		ImGui::GetCursorPosY() - vertical_offset + text_y_offset
	);

	if (b_IsPlaying)
	{
		ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "PLAYING");
	}
	else
	{
		ImGui::TextColored(ImVec4(0.8f, 0.2f, 0.2f, 1.0f), "STOPPED");
	}

	// Restore button with PNG icon
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 12);
	if
	(
		ImGui::ImageButton
		(
			"restore_btn",
			(ImTextureID)(intptr_t)m_RestoreIcon.id,
			ImVec2(28, 28)
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

	ImGui::SameLine();

	// Compile Button & Status
	float button_sz = 28.0f + ImGui::GetStyle().FramePadding.x * 2.0f;
	float status_sz = 0.0f;
	if (b_IsCompiling)
	{
		status_sz = 20.0f + ImGui::GetStyle().ItemSpacing.x + ImGui::CalcTextSize("Compiling...").x + ImGui::GetStyle().ItemSpacing.x;
	}

	float avail = ImGui::GetContentRegionAvail().x;
	float pos_x = ImGui::GetCursorPosX() + avail - button_sz - status_sz;
	if (pos_x > ImGui::GetCursorPosX())
	{
		ImGui::SetCursorPosX(pos_x);
	}

	if (b_IsCompiling)
	{
		// Center spinner and text vertically in toolbar
		float spinner_height = 20.0f;
		float spinner_y_offset = (toolbar_height - spinner_height) / 2.0f;
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - vertical_offset + spinner_y_offset);

		DrawSpinner(10.0f, 2.0f, ImGui::GetColorU32(ImVec4(0.2f, 0.8f, 0.2f, 1.0f)));
		
		ImGui::SameLine();
		// Center text vertically in toolbar
		float text_compile_y_offset = (toolbar_height - ImGui::GetTextLineHeight()) / 2.0f;
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - vertical_offset + text_compile_y_offset);
		ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "Compiling...");
		ImGui::SameLine();
	}

	bool b_Disabled = b_IsCompiling.load();
	if (b_Disabled)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
	}

	if
	(
		ImGui::ImageButton
		(
			"compile_btn",
			(ImTextureID)(intptr_t)m_CompileIcon.id,
			ImVec2(28, 28)
		)
	)
	{
		if (!b_Disabled)
		{
			b_IsCompiling = true;

			std::thread
			(
				[this]()
				{
					constexpr int BUILD_TIMEOUT_SECONDS = 60; // 60 seconds
					bool b_Success = b_StartBuildProcessWithTimeout
					(
						"build_gamelogic.bat nopause",
						BUILD_TIMEOUT_SECONDS
					);

					b_IsCompiling = false;
				}
			).detach();
		}
	}

	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip("Recompile Game Logic DLL");
	}

	if (b_Disabled)
	{
		ImGui::PopStyleVar();
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
        MapManager* map_manager = dynamic_cast<MapManager*>(game_map.get());

        if (map_manager)
        {
            // If it's a MapManager, set it using the dedicated method
            std::unique_ptr<MapManager> OwnedMapManager
			(
				static_cast<MapManager*>(game_map.release())
			);

            m_GameEngine.SetMapManager(std::move(OwnedMapManager));
            
            // Store reference for map selection UI
            m_MapManager = m_GameEngine.GetMapManager();
        }
        else
        {
            // Otherwise, use the regular SetMap method
            m_GameEngine.SetMap(std::move(game_map));
            m_MapManager = nullptr; // No MapManager available
        }
    }
    else
    {
        m_GameEngine.SetMap(std::make_unique<GameMap>());
        m_MapManager = nullptr;
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
		std::unique_ptr<MapManager> OwnedMapManager
		(
			static_cast<MapManager*>(new_map.release())
		);
		m_GameEngine.SetMapManager(std::move(OwnedMapManager));
		
		// Store reference for map selection UI
		m_MapManager = m_GameEngine.GetMapManager();
	}
	else
	{
		// Otherwise, use the regular SetMap method
		m_GameEngine.SetMap(std::move(new_map));
		m_MapManager = nullptr; // No MapManager available
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

void GameEditor::DrawMapSelectionUI()
{
	// Only show map selection when game is paused and we have a MapManager
	if (!m_MapManager)
	{
		return;
	}

	ImGui::Begin("Map Selection", nullptr, ImGuiWindowFlags_NoCollapse);

	ImGui::Text("Current Map: %s", m_MapManager->GetCurrentMapId().c_str());
	ImGui::Separator();
	ImGui::Spacing();

	// Get available maps
	auto available_maps = m_MapManager->GetAvailableMaps();

	if (available_maps.empty())
	{
		ImGui::TextColored
		(
			ImVec4(1.0f, 0.6f, 0.6f, 1.0f), 
			"No maps registered in MapManager"
		);
		ImGui::Text("Register maps using RegisterMap<YourMap>(\"MAP_ID\")");
	}
	else
	{
		ImGui::Text("Available Maps:");
		ImGui::Spacing();

		// Create a combo box for map selection
		static int s_SelectedIndex = 0;
		std::string curr_map_id = m_MapManager->GetCurrentMapId();

		// Find current map index
		for (int i = 0; i < available_maps.size(); i++)
		{
			if (available_maps[i] == curr_map_id)
			{
				s_SelectedIndex = i;
				break;
			}
		}

		// Create combo box
		if
		(
			ImGui::BeginCombo
			(
				"Select Map", 
				curr_map_id.empty() ? 
				"No map loaded" : curr_map_id.c_str()
			)
		)
		{
			for (int i = 0; i < available_maps.size(); i++)
			{
				bool b_IsSelected = (s_SelectedIndex == i);
				bool b_IsCurrent = (available_maps[i] == curr_map_id);

				// Highlight current map in red
				if (b_IsCurrent)
				{
					ImGui::PushStyleColor
					(
						ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f) // Red color
					);
				}
				// Highlight MainMap (index 0) in a special way
				else if (i == 0)
				{
					ImGui::PushStyleColor
					(
						ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.2f, 1.0f) // Yellow/gold color
					);
				}

				if(ImGui::Selectable(available_maps[i].c_str(), b_IsSelected))
				{
					s_SelectedIndex = i;
					m_SelectedMapId = available_maps[i];

					// Switch to selected map
					if (m_SelectedMapId != curr_map_id)
					{
						m_MapManager->b_GotoMap(m_SelectedMapId);
					}
				}

				// If the item is focused, set it as the default
				if (b_IsSelected)
				{
					ImGui::SetItemDefaultFocus();
				}

				// Pop style color if we pushed it
				if (b_IsCurrent || i == 0)
				{
					ImGui::PopStyleColor();
				}
			}
			ImGui::EndCombo();
		}

		ImGui::Spacing();

		// Quick access buttons for each map
		ImGui::Text("Quick Access:");
		for (int i = 0; i < available_maps.size(); i++)
		{
			const auto& MAP_ID = available_maps[i];
			bool b_IsCurrent = (MAP_ID == curr_map_id);

			// Style current map button with red color
			if (b_IsCurrent)
			{
				ImGui::PushStyleColor
				(
					ImGuiCol_Button, ImVec4(0.7f, 0.2f, 0.2f, 0.6f) // Dark red
				);

				ImGui::PushStyleColor
				(
					ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.3f, 0.3f, 0.8f) // Light red
				);

				ImGui::PushStyleColor
				(
					ImGuiCol_ButtonActive, ImVec4(0.5f, 0.1f, 0.1f, 1.0f) // Darker red
				);
			}
			// Style MainMap (index 0) button differently to make it stand out
			else if (i == 0)
			{
				ImGui::PushStyleColor
				(
					ImGuiCol_Button, ImVec4(0.8f, 0.6f, 0.0f, 0.7f) // Dark gold
				);

				ImGui::PushStyleColor
				(
					ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.8f, 0.2f, 0.9f) // Light gold
				);

				ImGui::PushStyleColor
				(
					ImGuiCol_ButtonActive, ImVec4(0.6f, 0.4f, 0.0f, 1.0f) // Darker gold
				);
			}

			// Add a special label for MainMap
			std::string button_label = MAP_ID;
			if (i == 0)
			{
				button_label += " (Main)";
			}

			if (ImGui::Button(button_label.c_str(), ImVec2(-1, 0)))
			{
				if (MAP_ID != curr_map_id)
				{
					m_MapManager->b_GotoMap(MAP_ID);
				}
			}

			// Pop style colors if we pushed them
			if (b_IsCurrent || i == 0)
			{
				ImGui::PopStyleColor(3);
			}
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
	}

	ImGui::End();
}