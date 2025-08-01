#include "EditorPanels.h"
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

SceneViewPanel::SceneViewPanel(GameObject* gameObjects, int& objectCount, int& selectedObject) 
    : m_gameObjects(gameObjects), m_objectCount(objectCount), m_selectedObject(selectedObject) {}

void SceneViewPanel::Render() {
    ImGui::Begin("Scene View");
    ImVec2 viewSize = ImGui::GetContentRegionAvail();
    ImGui::Text("Scene Viewport (%dx%d)", (int)viewSize.x, (int)viewSize.y);
    ImGui::Text("Right-click to interact");
    // Simple scene representation
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImVec2 canvasSize = ImGui::GetContentRegionAvail();

    if (canvasSize.x > 50 && canvasSize.y > 50) {
        drawList->AddRectFilled(canvasPos, ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y), IM_COL32(50, 50, 50, 255));
        for (int i = 0; i < m_objectCount; i++) {
            float x = canvasPos.x + canvasSize.x * 0.5f + m_gameObjects[i].position[0] * 20;
            float y = canvasPos.y + canvasSize.y * 0.5f - m_gameObjects[i].position[1] * 20;
            ImU32 color = (i == m_selectedObject) ? IM_COL32(255, 255, 0, 255) : IM_COL32(100, 150, 255, 255);
            drawList->AddCircleFilled(ImVec2(x, y), 8, color);
            drawList->AddText(ImVec2(x + 12, y - 8), IM_COL32(255, 255, 255, 255), m_gameObjects[i].name);
        }
    }
    ImGui::End();
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
