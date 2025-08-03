#include "EditorPanels.h"

// Forward declare the t_GameObject struct (defined in GameEditor.cpp)
struct t_GameObject
{
    char name[64];
    float position[3];
    float rotation[3];
    float scale[3];
    bool b_Enabled;
};

MenuBarPanel::MenuBarPanel(char *log_buffer, bool &b_ShowDemo) : m_LogBuffer(log_buffer), bm_ShowDemo(b_ShowDemo) {}

void MenuBarPanel::Render()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Scene", "Ctrl+N"))
            {
                strcat_s(m_LogBuffer, 1024, "New scene created\n");
            }
            if (ImGui::MenuItem("Open Scene", "Ctrl+O"))
            {
                strcat_s(m_LogBuffer, 1024, "Scene loading...\n");
            }
            if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
            {
                strcat_s(m_LogBuffer, 1024, "Scene saved\n");
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit"))
            {
                // Close window - could set a flag here
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z"))
            {
                // Undo functionality
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y"))
            {
                // Redo functionality
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Show Demo", NULL, &bm_ShowDemo);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

HierarchyPanel::HierarchyPanel(t_GameObject *game_objects, int &object_count, int &selected_object, char *log_buffer)
    : m_GameObjects(game_objects), m_ObjectCount(object_count), m_SelectedObject(selected_object), m_LogBuffer(log_buffer) {}

void HierarchyPanel::Render()
{
    ImGui::Begin("Scene Hierarchy");
    ImGui::Text("Objects: %d", m_ObjectCount);
    ImGui::Separator();
    for (int i = 0; i < m_ObjectCount; i++)
    {
        bool b_IsSelected = (m_SelectedObject == i);
        if (ImGui::Selectable(m_GameObjects[i].name, b_IsSelected))
        {
            m_SelectedObject = i;
        }
    }
    ImGui::Separator();
    if (ImGui::Button("Add Object"))
    {
        if (m_ObjectCount < 10)
        {
            sprintf_s(m_GameObjects[m_ObjectCount].name, "GameObject_%d", m_ObjectCount);
            m_ObjectCount++;
            strcat_s(m_LogBuffer, 1024, "New object added\n");
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete") && m_ObjectCount > 0)
    {
        m_ObjectCount--;
        strcat_s(m_LogBuffer, 1024, "Object deleted\n");
    }
    ImGui::End();
}

InspectorPanel::InspectorPanel(t_GameObject *game_objects, int &object_count, int &selected_object)
    : m_GameObjects(game_objects), m_ObjectCount(object_count), m_SelectedObject(selected_object) {}

void InspectorPanel::Render()
{
    ImGui::Begin("Inspector");
    if (m_SelectedObject < m_ObjectCount)
    {
        t_GameObject &obj = m_GameObjects[m_SelectedObject];
        ImGui::Text("Selected: %s", obj.name);
        ImGui::Separator();
        ImGui::InputText("Name", obj.name, sizeof(obj.name));
        ImGui::Checkbox("Enabled", &obj.b_Enabled);
        ImGui::Spacing();
        ImGui::Text("Transform");
        ImGui::DragFloat3("Position", obj.position, 0.1f);
        ImGui::DragFloat3("Rotation", obj.rotation, 1.0f);
        ImGui::DragFloat3("Scale", obj.scale, 0.1f);
        ImGui::Spacing();
        if (ImGui::Button("Reset Transform"))
        {
            obj.position[0] = obj.position[1] = obj.position[2] = 0.0f;
            obj.rotation[0] = obj.rotation[1] = obj.rotation[2] = 0.0f;
            obj.scale[0] = obj.scale[1] = obj.scale[2] = 1.0f;
        }
    }
    else
    {
        ImGui::Text("No object selected");
    }
    ImGui::End();
}

SceneViewPanel::SceneViewPanel(t_GameObject *game_objects, int &object_count, int &selected_object, CoreEngine *core_engine)
    : m_GameObjects(game_objects), m_ObjectCount(object_count), m_SelectedObject(selected_object),
      m_CoreEngine(core_engine), m_RenderTexture({0}), bm_RenderTextureInitialized(false),
      m_SceneWidth(800), m_SceneHeight(600), m_GameLogic(nullptr), bm_GameInitialized(false)
{
    // Initialize game logic
    if (m_CoreEngine)
    {
        m_GameLogic = new GameLogic(m_CoreEngine);
    }
}

SceneViewPanel::~SceneViewPanel()
{
    if (bm_RenderTextureInitialized && m_CoreEngine)
    {
        m_CoreEngine->UnloadRenderTexture(m_RenderTexture);
    }

    // Clean up game logic
    delete m_GameLogic;
    m_GameLogic = nullptr;
}

void SceneViewPanel::Render()
{
    ImGui::Begin("Scene View");
    ImVec2 viewSize = ImGui::GetContentRegionAvail();
    m_SceneWidth = (int)viewSize.x;
    m_SceneHeight = (int)viewSize.y;

    if 
    (   
        !bm_RenderTextureInitialized ||
        m_RenderTexture.texture.width != m_SceneWidth ||
        m_RenderTexture.texture.height != m_SceneHeight
    )
    {
        InitializeRenderTexture(m_SceneWidth, m_SceneHeight);
    }

    RenderScene();

    // Play/Stop controls
    if (ImGui::Button(m_GameLogic->IsRunning() ? "Stop" : "Play", ImVec2(80, 25)))
    {
        m_GameLogic->SetRunning(!m_GameLogic->IsRunning());
        m_CoreEngine->SetSimulationState(m_GameLogic->IsRunning());
    }

    // Only display the texture if it's properly initialized
    if (bm_RenderTextureInitialized && m_RenderTexture.texture.id != 0)
    {
        ImGui::Image((ImTextureID)m_RenderTexture.texture.id, viewSize, ImVec2(0, 1), ImVec2(1, 0));
    }
    else
    {
        ImGui::Text("Initializing scene view...");
    }
    ImGui::End();
}

void SceneViewPanel::Update()
{
    HandleSceneInput();
}

void SceneViewPanel::InitializeRenderTexture(int width, int height)
{
    if (bm_RenderTextureInitialized && m_CoreEngine)
    {
        m_CoreEngine->UnloadRenderTexture(m_RenderTexture);
    }
    if (m_CoreEngine)
    {
        m_RenderTexture = m_CoreEngine->LoadRenderTexture(width, height);
        bm_RenderTextureInitialized = true;
    }
}

void SceneViewPanel::RenderScene()
{
    if (!bm_RenderTextureInitialized)
    {
        return; // Don't render if texture isn't initialized
    }

    // Initialize game logic if not done yet
    if (m_GameLogic && !bm_GameInitialized)
    {
        m_GameLogic->SetSceneBounds((float)m_SceneWidth, (float)m_SceneHeight);
        m_GameLogic->Init();
        bm_GameInitialized = true;
    }
    else if (m_GameLogic)
    {
        // Update scene bounds in case the scene view was resized
        m_GameLogic->SetSceneBounds((float)m_SceneWidth, (float)m_SceneHeight);
    }

    // Begin rendering to texture
    if (m_CoreEngine)
    {
        m_CoreEngine->BeginTextureMode(m_RenderTexture);
        m_CoreEngine->ClearScreen(DARKBLUE);

        // Render the game logic
        if (m_GameLogic)
        {
            m_GameLogic->Render();
        }

        // Render editor overlay information
        m_CoreEngine->DrawText("GAME PREVIEW - Live in Editor", 10, m_SceneHeight - 40, 14, LIME);
        m_CoreEngine->DrawText("Edit GameLogic.cpp to modify this game", 10, m_SceneHeight - 20, 12, LIGHTGRAY);

        m_CoreEngine->EndTextureMode();
    }
}

void SceneViewPanel::HandleSceneInput()
{
    // Update game logic
    if (m_GameLogic && bm_GameInitialized && m_CoreEngine && m_GameLogic->IsRunning())
    {
        float DeltaTime = GetFrameTime();
        m_GameLogic->Update(DeltaTime);
    }
}

ConsolePanel::ConsolePanel(char *log_buffer) : m_LogBuffer(log_buffer) {}

void ConsolePanel::Render()
{
    ImGui::Begin("Console");
    ImGui::TextWrapped("%s", m_LogBuffer);
    if (ImGui::Button("Clear"))
    {
        m_LogBuffer[0] = '\0';
    }
    ImGui::SameLine();
    if (ImGui::Button("Test Log"))
    {
        strcat_s(m_LogBuffer, 1024, "Test message added\n");
    }
    ImGui::End();
}

AssetsPanel::AssetsPanel() {}

void AssetsPanel::Render()
{
    ImGui::Begin("Assets");
    ImGui::Text("Project Assets");
    ImGui::Separator();
    const char *ASSET_TYPES[] = 
    {
        "Textures", "Models", "Scripts", "Audio", "Materials"
    };
    for (int i = 0; i < 5; i++)
    {
        if (ImGui::TreeNode(ASSET_TYPES[i]))
        {
            ImGui::Text("  - asset_example_%d", i + 1);
            ImGui::Text("  - asset_example_%d", i + 2);
            ImGui::TreePop();
        }
    }
    ImGui::End();
}
