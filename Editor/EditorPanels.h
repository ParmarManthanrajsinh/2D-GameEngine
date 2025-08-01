#pragma once

#include <imgui.h>
#include <string>

// Forward declarations
struct GameObject;

class MenuBarPanel {
public:
    MenuBarPanel(char* logBuffer, bool& showDemo);
    void Render();

private:
    char* m_logBuffer;
    bool& m_showDemo;
};

class HierarchyPanel {
public:
    HierarchyPanel(GameObject* gameObjects, int& objectCount, int& selectedObject, char* logBuffer);
    void Render();

private:
    GameObject* m_gameObjects;
    int& m_objectCount;
    int& m_selectedObject;
    char* m_logBuffer;
};

class InspectorPanel {
public:
    InspectorPanel(GameObject* gameObjects, int& objectCount, int& selectedObject);
    void Render();

private:
    GameObject* m_gameObjects;
    int& m_objectCount;
    int& m_selectedObject;
};

class SceneViewPanel {
public:
    SceneViewPanel(GameObject* gameObjects, int& objectCount, int& selectedObject);
    void Render();

private:
    GameObject* m_gameObjects;
    int& m_objectCount;
    int& m_selectedObject;
};

class ConsolePanel {
public:
    ConsolePanel(char* logBuffer);
    void Render();

private:
    char* m_logBuffer;
};

class AssetsPanel {
public:
    AssetsPanel();
    void Render();
};
