#pragma once

#include <imgui.h>
#include <raylib.h>
#include <string>
#include <cstring>
#include <cstdio>
#include "CoreEngine.h"
#include "GameLogic.h"

// Forward declarations
struct GameObject;
class CoreEngine;
class GameLogic;

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
    SceneViewPanel(GameObject* gameObjects, int& objectCount, int& selectedObject, CoreEngine* coreEngine);
    ~SceneViewPanel();
    void Render();
    void Update(); // Handle input and scene updates

private:
    GameObject* m_gameObjects;
    int& m_objectCount;
    int& m_selectedObject;
    
    // Rendering
    CoreEngine* m_coreEngine;  // Pointer to core engine for rendering and input

    RenderTexture2D m_renderTexture;
    bool m_renderTextureInitialized;
    int m_sceneWidth;
    int m_sceneHeight;
    
    // Game logic integration
    GameLogic* m_gameLogic;
    bool m_gameInitialized;
    
    void InitializeRenderTexture(int width, int height);
    void RenderScene();
    void HandleSceneInput();
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
