#include "GameEditor.h"

// Simple game object structure for demo
struct GameObject
{
    char name[64] = "";
    float position[3] = { 0.0f, 0.0f, 0.0f };
    float rotation[3] = { 0.0f, 0.0f, 0.0f };
    float scale[3] = { 1.0f, 1.0f, 1.0f };
    bool enabled = true;
};

class GameEditor::Impl {
public:
    // Demo data
    GameObject gameObjects[10];
    int objectCount = 3;
    int selectedObject = 0;
    bool showDemo = false;
    char logBuffer[1024] = "Engine initialized successfully\nReady to create!\n";
    ImGuiViewport* viewport = nullptr;

    // Core engine
    CoreEngine coreEngine;

    // Panel instances for better modularity
    std::unique_ptr<MenuBarPanel> menuBarPanel;
    std::unique_ptr<HierarchyPanel> hierarchyPanel;
    std::unique_ptr<InspectorPanel> inspectorPanel;
    std::unique_ptr<SceneViewPanel> sceneViewPanel;
    std::unique_ptr<ConsolePanel> consolePanel;
    std::unique_ptr<AssetsPanel> assetsPanel;

    Impl() {
        // Initialize some demo objects
        strcpy_s(gameObjects[0].name, "Player");
        strcpy_s(gameObjects[1].name, "Camera");
        strcpy_s(gameObjects[2].name, "Light");

        gameObjects[1].position[1] = 5.0f;
        gameObjects[2].position[0] = 2.0f;
        gameObjects[2].position[1] = 3.0f;

        // CoreEngine initialization will be done in GameEditor::Init()

        // Initialize panels
        menuBarPanel = std::make_unique<MenuBarPanel>(logBuffer, showDemo);
        hierarchyPanel = std::make_unique<HierarchyPanel>(gameObjects, objectCount, selectedObject, logBuffer);
        inspectorPanel = std::make_unique<InspectorPanel>(gameObjects, objectCount, selectedObject);
        sceneViewPanel = std::make_unique<SceneViewPanel>(gameObjects, objectCount, selectedObject, &coreEngine);
        consolePanel = std::make_unique<ConsolePanel>(logBuffer);
        assetsPanel = std::make_unique<AssetsPanel>();
    }
};

GameEditor::GameEditor(int width, int height, const char* title) 
    : pImpl(std::make_unique<Impl>()) {
    Init(width, height, title);
}

GameEditor::~GameEditor() {
    Close();
}

void GameEditor::Init(int width, int height, const char* title) {
    // Initialize core engine (this will create the raylib window)
    if (!pImpl->coreEngine.Initialize(width, height, std::string(title))) {
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
    pImpl->viewport = ImGui::GetMainViewport();
}

void GameEditor::Run() {
    while (!WindowShouldClose())
    {
        pImpl->coreEngine.BeginFrame();
        pImpl->coreEngine.ClearScreen(DARKGRAY);

        rlImGuiBegin();

        // Create dockspace
        ImGui::DockSpaceOverViewport(0, pImpl->viewport);

        // Update scene panel for input handling
        pImpl->sceneViewPanel->Update();
        
        // Render all UI panels using dedicated panel classes
        pImpl->menuBarPanel->Render();
        pImpl->hierarchyPanel->Render();
        pImpl->inspectorPanel->Render();
        pImpl->sceneViewPanel->Render();
        pImpl->consolePanel->Render();
        pImpl->assetsPanel->Render();

        // Show ImGui demo if requested
        if (pImpl->showDemo)
        {
            ImGui::ShowDemoWindow(&pImpl->showDemo);
        }

        rlImGuiEnd();
        pImpl->coreEngine.EndFrame();
    }
}

void GameEditor::Close() {
    rlImGuiShutdown();
    pImpl->coreEngine.Shutdown();
}
