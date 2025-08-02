#pragma once

#include <raylib.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Forward declarations
struct RenderObject;

/**
 * Core Engine Foundation - Milestone 1
 *
 * This class provides the basic window management and rendering capabilities
 * using raylib. 
 * 
 * It handles:
 * 1. Window initialization with title and dimensions
 * 2. Basic drawing operations (shapes, sprites, textures)
 * 3. Screen clearing each frame
 */
class CoreEngine
{
public:
    void SetSimulationState(bool running) { m_IsRunning = running; }
    bool IsRunning() const { return m_IsRunning; }

private:
    bool m_IsRunning = false;
public:
    CoreEngine();
    ~CoreEngine();

    // Core initialization and cleanup
    bool Initialize(int width, int height, const std::string &title);
    void Shutdown();

    // Main loop control
    bool ShouldClose() const;
    void BeginFrame();
    void EndFrame();

    // Basic drawing operations
    void ClearScreen(Color color = RAYWHITE);
    void DrawRectangle(int x, int y, int width, int height, Color color);
    void DrawCircle(int centerX, int centerY, float radius, Color color);
    void DrawTexture(Texture2D texture, int x, int y, Color tint = WHITE);
    void DrawSprite(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint = WHITE);
    void DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color);
    void DrawText(const char *text, int posX, int posY, int fontSize, Color color);

    // Texture management
    Texture2D LoadTextureFromFile(const std::string &filename);
    void UnloadTexture(Texture2D texture);
    RenderTexture2D LoadRenderTexture(int width, int height);
    void UnloadRenderTexture(RenderTexture2D target);
    void BeginTextureMode(RenderTexture2D target);
    void EndTextureMode();

    // Window properties
    int GetScreenWidth() const;
    int GetScreenHeight() const;

    // Collision detection
    bool CheckCollisionRecs(Rectangle rec1, Rectangle rec2) const;
    bool CheckCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2) const;
    bool CheckCollisionCircleRec(Vector2 center, float radius, Rectangle rec) const;
    bool CheckCollisionPointRec(Vector2 point, Rectangle rec) const;
    bool CheckCollisionPointCircle(Vector2 point, Vector2 center, float radius) const;

    // Render object management (for basic scene rendering)
    void AddRenderObject(const RenderObject &obj);
    void ClearRenderObjects();
    void RenderAllObjects();

private:
    bool m_initialized;
    int m_screenWidth;
    int m_screenHeight;
    std::string m_windowTitle;
    std::vector<RenderObject> m_renderObjects;
    std::vector<Texture2D> m_loadedTextures; // For cleanup tracking
};

// Simple render object structure for basic rendering
struct RenderObject
{
    enum Type
    {
        RECTANGLE,
        CIRCLE,
        TEXTURE,
        SPRITE
    };

    Type type;
    Vector2 position;
    Color color;

    // Rectangle specific
    Vector2 size;

    // Circle specific
    float radius;

    // Texture/Sprite specific
    Texture2D texture;
    Rectangle sourceRect; // For sprite sheets
    Rectangle destRect;   // Destination rectangle
    Vector2 origin;       // Origin point for rotation
    float rotation;       // Rotation in degrees
    Color tint;           // Texture tinting

    // Constructors for different types
    static RenderObject CreateRectangle(float x, float y, float width, float height, Color color);
    static RenderObject CreateCircle(float x, float y, float radius, Color color);
    static RenderObject CreateTexture(Texture2D tex, float x, float y, Color tint = WHITE);
    static RenderObject CreateSprite(Texture2D tex, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint = WHITE);
};
