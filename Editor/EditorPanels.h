#pragma once

#include <imgui.h>
#include <raylib.h>
#include <string>
#include <cstring>
#include <cstdio>
#include "CoreEngine.h"
#include "GameLogic.h"

// Forward declarations
struct t_GameObject;
class CoreEngine;
class GameLogic;

class MenuBarPanel
{
public:
    MenuBarPanel(char *log_buffer, bool &b_ShowDemo);
    void Render();

private:
    char *m_LogBuffer;
    bool &bm_ShowDemo;
};

class HierarchyPanel
{
public:
    HierarchyPanel(t_GameObject *gameObjects, int &ObjectCount, int &selected_object, char *log_buffer);
    void Render();

private:
    t_GameObject *m_GameObjects;
    int &m_ObjectCount;
    int &m_SelectedObject;
    char *m_LogBuffer;
};

class InspectorPanel
{
public:
    InspectorPanel(t_GameObject *gameObjects, int &ObjectCount, int &selected_object);
    void Render();

private:
    t_GameObject *m_GameObjects;
    int &m_ObjectCount;
    int &m_SelectedObject;
};

class SceneViewPanel
{
public:
    SceneViewPanel(t_GameObject *gameObjects, int &ObjectCount, int &selected_object, CoreEngine *core_engine);
    ~SceneViewPanel();
    void Render();
    void Update(); 

private:
    t_GameObject *m_GameObjects;
    int &m_ObjectCount;
    int &m_SelectedObject;

    // Rendering
    CoreEngine *m_CoreEngine; // Pointer to core engine for rendering and input

    RenderTexture2D m_RenderTexture;
    bool bm_RenderTextureInitialized;
    int m_SceneWidth;
    int m_SceneHeight;

    // Game logic integration
    GameLogic *m_GameLogic;
    bool bm_GameInitialized;

    void InitializeRenderTexture(int width, int height);
    void RenderScene();
    void HandleSceneInput();
};

class ConsolePanel
{
public:
    ConsolePanel(char *log_buffer);
    void Render();

private:
    char *m_LogBuffer;
};

class AssetsPanel
{
public:
    AssetsPanel();
    void Render();
};
