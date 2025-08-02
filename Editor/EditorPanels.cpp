#include "EditorPanels.h"
#include "CoreEngine.h"
#include "GameLogic.h"
#include <cstring>
#include <cstdio>

// Forward declare the GameObject struct (defined in GameEditor.cpp)
struct GameObject {
    char name[64];
    float position[3];
    float rotation[3];
    float scale[3];
    bool enabled;
};

MenuBarPanel::MenuBarPanel(char* logBuffer, bool& showDemo) : m_logBuffer(logBuffer), m_showDemo(showDemo) {}

void MenuBarPanel::Render() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
                strcat(m_logBuffer, "New scene created\n");
            }
            if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {
                strcat(m_logBuffer, "Scene loading...\n");
            }
            if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
                strcat(m_logBuffer, "Scene saved\n");
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) {
                // Close window - could set a flag here
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
                // Undo functionality
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
                // Redo functionality
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Show Demo", NULL, &m_showDemo);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

HierarchyPanel::HierarchyPanel(GameObject* gameObjects, int& objectCount, int& selectedObject, char* logBuffer) 
    : m_gameObjects(gameObjects), m_objectCount(objectCount), m_selectedObject(selectedObject), m_logBuffer(logBuffer) {}

void HierarchyPanel::Render() {
    ImGui::Begin("Scene Hierarchy");
    ImGui::Text("Objects: %d", m_objectCount);
    ImGui::Separator();
    for (int i = 0; i < m_objectCount; i++) {
        bool isSelected = (m_selectedObject == i);
        if (ImGui::Selectable(m_gameObjects[i].name, isSelected)) {
            m_selectedObject = i;
        }
    }
    ImGui::Separator();
    if (ImGui::Button("Add Object")) {
        if (m_objectCount < 10) {
            sprintf_s(m_gameObjects[m_objectCount].name, "GameObject_%d", m_objectCount);
            m_objectCount++;
            strcat(m_logBuffer, "New object added\n");
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete") && m_objectCount > 0) {
       m_objectCount--;
       strcat(m_logBuffer, "Object deleted\n");
    }
    ImGui::End();
}

InspectorPanel::InspectorPanel(GameObject* gameObjects, int& objectCount, int& selectedObject) 
    : m_gameObjects(gameObjects), m_objectCount(objectCount), m_selectedObject(selectedObject) {}

void InspectorPanel::Render() {
    ImGui::Begin("Inspector");
    if (m_selectedObject < m_objectCount) {
        GameObject& obj = m_gameObjects[m_selectedObject];
        ImGui::Text("Selected: %s", obj.name);
        ImGui::Separator();
        ImGui::InputText("Name", obj.name, sizeof(obj.name));
        ImGui::Checkbox("Enabled", &obj.enabled);
        ImGui::Spacing();
        ImGui::Text("Transform");
        ImGui::DragFloat3("Position", obj.position, 0.1f);
        ImGui::DragFloat3("Rotation", obj.rotation, 1.0f);
        ImGui::DragFloat3("Scale", obj.scale, 0.1f);
        ImGui::Spacing();
        if (ImGui::Button("Reset Transform")) {
            obj.position[0] = obj.position[1] = obj.position[2] = 0.0f;
            obj.rotation[0] = obj.rotation[1] = obj.rotation[2] = 0.0f;
            obj.scale[0] = obj.scale[1] = obj.scale[2] = 1.0f;
        }
    } else {
        ImGui::Text("No object selected");
    }
    ImGui::End();
}

SceneViewPanel::SceneViewPanel(GameObject* gameObjects, int& objectCount, int& selectedObject, CoreEngine* coreEngine) 
    : m_gameObjects(gameObjects), m_objectCount(objectCount), m_selectedObject(selectedObject),
      m_coreEngine(coreEngine), m_renderTexture({0}), m_renderTextureInitialized(false), 
      m_sceneWidth(800), m_sceneHeight(600), m_gameLogic(nullptr), m_gameInitialized(false)
{
    // Initialize game logic
    if (m_coreEngine) {
        m_gameLogic = new GameLogic(m_coreEngine);
    }
}

SceneViewPanel::~SceneViewPanel() {
    if (m_renderTextureInitialized && m_coreEngine) {
        m_coreEngine->UnloadRenderTexture(m_renderTexture);
    }
    
    // Clean up game logic
    delete m_gameLogic;
    m_gameLogic = nullptr;
}

void SceneViewPanel::Render() {
    ImGui::Begin("Scene View");
    ImVec2 viewSize = ImGui::GetContentRegionAvail();
    m_sceneWidth = (int)viewSize.x;
    m_sceneHeight = (int)viewSize.y;

    if (!m_renderTextureInitialized ||
        m_renderTexture.texture.width != m_sceneWidth ||
        m_renderTexture.texture.height != m_sceneHeight) {
        InitializeRenderTexture(m_sceneWidth, m_sceneHeight);
    }

    RenderScene();

    // Only display the texture if it's properly initialized
    if (m_renderTextureInitialized && m_renderTexture.texture.id != 0) {
        ImGui::Image((void*)m_renderTexture.texture.id, viewSize, ImVec2(0, 1), ImVec2(1, 0));
    } else {
        ImGui::Text("Initializing scene view...");
    }
    ImGui::End();
}

void SceneViewPanel::Update() {
    HandleSceneInput();
}

void SceneViewPanel::InitializeRenderTexture(int width, int height) {
    if (m_renderTextureInitialized && m_coreEngine) {
        m_coreEngine->UnloadRenderTexture(m_renderTexture);
    }
    if (m_coreEngine) {
        m_renderTexture = m_coreEngine->LoadRenderTexture(width, height);
        m_renderTextureInitialized = true;
    }
}

void SceneViewPanel::RenderScene() {
    if (!m_renderTextureInitialized) {
        return; // Don't render if texture isn't initialized
    }
    
    // Initialize game logic if not done yet
    if (m_gameLogic && !m_gameInitialized) {
        m_gameLogic->SetSceneBounds((float)m_sceneWidth, (float)m_sceneHeight);
        m_gameLogic->Init();
        m_gameInitialized = true;
    } else if (m_gameLogic) {
        // Update scene bounds in case the scene view was resized
        m_gameLogic->SetSceneBounds((float)m_sceneWidth, (float)m_sceneHeight);
    }
    
    // Begin rendering to texture
    if (m_coreEngine) {
        m_coreEngine->BeginTextureMode(m_renderTexture);
        m_coreEngine->ClearScreen(DARKBLUE);
        
        // Render the game logic
        if (m_gameLogic) {
            m_gameLogic->Render();
        }
        
        // Render editor overlay information
        m_coreEngine->DrawText("GAME PREVIEW - Live in Editor", 10, m_sceneHeight - 40, 14, LIME);
        m_coreEngine->DrawText("Edit GameLogic.cpp to modify this game", 10, m_sceneHeight - 20, 12, LIGHTGRAY);
        
        m_coreEngine->EndTextureMode();
    }
}

void SceneViewPanel::HandleSceneInput() {
    // Update game logic
    if (m_gameLogic && m_gameInitialized && m_coreEngine) {
        float deltaTime = GetFrameTime();
        m_gameLogic->Update(deltaTime);
    }
}

ConsolePanel::ConsolePanel(char* logBuffer) : m_logBuffer(logBuffer) {}

void ConsolePanel::Render() {
    ImGui::Begin("Console");
    ImGui::TextWrapped("%s", m_logBuffer);
    if (ImGui::Button("Clear")) {
        m_logBuffer[0] = '\0';
    }
    ImGui::SameLine();
    if (ImGui::Button("Test Log")) {
        strcat(m_logBuffer, "Test message added\n");
    }
    ImGui::End();
}

AssetsPanel::AssetsPanel() {}

void AssetsPanel::Render() {
    ImGui::Begin("Assets");
    ImGui::Text("Project Assets");
    ImGui::Separator();
    const char* assetTypes[] = { "Textures", "Models", "Scripts", "Audio", "Materials" };
    for (int i = 0; i < 5; i++) {
        if (ImGui::TreeNode(assetTypes[i])) {
            ImGui::Text("  - asset_example_%d", i + 1);
            ImGui::Text("  - asset_example_%d", i + 2);
            ImGui::TreePop();
        }
    }
    ImGui::End();
}
