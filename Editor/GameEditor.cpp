#include "GameEditor.h"

// Simple game object structure for demo
struct t_GameObject
{
    char name[64] = "";
    float position[3] = { 0.0f, 0.0f, 0.0f };
    float rotation[3] = { 0.0f, 0.0f, 0.0f };
    float scale[3] = { 1.0f, 1.0f, 1.0f };
    bool b_Enabled = true;
};

class GameEditor::Impl 
{
public:
    // Demo data
    t_GameObject game_objects[10];
    int object_count = 3;
    int selected_object = 0;
    bool b_ShowDemo = false;
    char log_buffer[1024] = "Engine initialized successfully\nReady to create!\n";
    ImGuiViewport* viewport = nullptr;

    // Core engine
    CoreEngine core_engine;

    // Panel instances for better modularity
    std::unique_ptr<MenuBarPanel> menu_bar_panel;
    std::unique_ptr<HierarchyPanel> hierarchy_panel;
    std::unique_ptr<InspectorPanel> inspector_panel;
    std::unique_ptr<SceneViewPanel> scene_view_panel;
    std::unique_ptr<ConsolePanel> console_panel;
    std::unique_ptr<AssetsPanel> assets_panel;

    Impl() 
    {
        // Initialize some demo objects
        strcpy_s(game_objects[0].name, "Player");
        strcpy_s(game_objects[1].name, "Camera");
        strcpy_s(game_objects[2].name, "Light");

        game_objects[1].position[1] = 5.0f;
        game_objects[2].position[0] = 2.0f;
        game_objects[2].position[1] = 3.0f;

        // CoreEngine initialization will be done in GameEditor::Init()

        // Initialize panels
        menu_bar_panel = std::make_unique<MenuBarPanel>(log_buffer, b_ShowDemo);
        hierarchy_panel = std::make_unique<HierarchyPanel>(game_objects, object_count, selected_object, log_buffer);
        inspector_panel = std::make_unique<InspectorPanel>(game_objects, object_count, selected_object);
        scene_view_panel = std::make_unique<SceneViewPanel>(game_objects, object_count, selected_object, &core_engine);
        console_panel = std::make_unique<ConsolePanel>(log_buffer);
        assets_panel = std::make_unique<AssetsPanel>();
    }
};

GameEditor::GameEditor(int width, int height, const char* title) 
    : PImpl(std::make_unique<Impl>()) 
{
    Init(width, height, title);
}

GameEditor::~GameEditor() 
{
    Close();
}

void GameEditor::Init(int width, int height, const char* title) 
{
    // Initialize core engine (this will create the raylib window)
    if (!PImpl->core_engine.b_Initialize(width, height, std::string(title))) 
    {
        std::cerr << "Failed to initialize CoreEngine!" << std::endl;
        return;
    }
    
    rlImGuiSetup(true);

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    rlImGuiReloadFonts();

    SetEngineTheme();
    LoadEditorDefaultIni();
    
    SetTargetFPS(60);
    PImpl->viewport = ImGui::GetMainViewport();
}

void GameEditor::Run() 
{
    while (!WindowShouldClose())
    {
        PImpl->core_engine.BeginFrame();
        PImpl->core_engine.ClearScreen(DARKGRAY);

        rlImGuiBegin();

        // Create dockspace
        ImGui::DockSpaceOverViewport(0, PImpl->viewport);

        // Update scene panel for input handling
        PImpl->scene_view_panel->Update();
        
        // Render all UI panels using dedicated panel classes
        PImpl->menu_bar_panel->Render();
        PImpl->hierarchy_panel->Render();
        PImpl->inspector_panel->Render();
        PImpl->scene_view_panel->Render();
        PImpl->console_panel->Render();
        PImpl->assets_panel->Render();

        // Show ImGui demo if requested
        if (PImpl->b_ShowDemo)
        {
            ImGui::ShowDemoWindow(&PImpl->b_ShowDemo);
        }

        rlImGuiEnd();
        PImpl->core_engine.EndFrame();
    }
}

void GameEditor::Close() 
{
    rlImGuiShutdown();
    PImpl->core_engine.Shutdown();
}