#include "CoreEngine.h"

// CoreEngine Implementation
CoreEngine::CoreEngine()
    : m_initialized(false), m_screenWidth(800), m_screenHeight(600),           m_windowTitle("2D Game Engine")
{
}

CoreEngine::~CoreEngine()
{
    if (m_initialized)
    {
        Shutdown();
    }
}

bool CoreEngine::Initialize(int width, int height, const std::string &title)
{
    if (m_initialized)
    {
        std::cout << "CoreEngine: Already initialized!" << std::endl;
        return false;
    }

    m_screenWidth = width;
    m_screenHeight = height;
    m_windowTitle = title;

    // Initialize raylib window
    InitWindow(width, height, title.c_str());

    if (!IsWindowReady())
    {
        std::cout << "CoreEngine: Failed to initialize window!" << std::endl;
        return false;
    }

    // Set default target FPS
    ::SetTargetFPS(60);

    m_initialized = true;

    std::cout << "CoreEngine: Initialized successfully" << std::endl;
    std::cout << "  - Window: " << width << "x" << height << " '" << title << "'" << std::endl;
    std::cout << "  - Raylib Version: " << RAYLIB_VERSION << std::endl;

    return true;
}

void CoreEngine::Shutdown()
{
    if (!m_initialized)
    {
        return;
    }

    // Unload all tracked textures
    for (auto &texture : m_loadedTextures)
    {
        UnloadTexture(texture);
    }
    m_loadedTextures.clear();

    // Clear render objects
    ClearRenderObjects();

    // Close raylib window
    CloseWindow();

    m_initialized = false;
    std::cout << "CoreEngine: Shutdown complete" << std::endl;
}

bool CoreEngine::ShouldClose() const
{
    return WindowShouldClose();
}

void CoreEngine::BeginFrame()
{
    if (!m_initialized)
        return;
    BeginDrawing();
}

void CoreEngine::EndFrame()
{
    if (!m_initialized)
        return;
    EndDrawing();
}

void CoreEngine::ClearScreen(Color color)
{
    if (!m_initialized)
        return;
    ClearBackground(color);
}

void CoreEngine::DrawRectangle(int x, int y, int width, int height, Color color)
{
    if (!m_initialized)
        return;
    ::DrawRectangle(x, y, width, height, color);
}

void CoreEngine::DrawCircle(int centerX, int centerY, float radius, Color color)
{
    if (!m_initialized)
        return;
    ::DrawCircle(centerX, centerY, radius, color);
}

void CoreEngine::DrawTexture(Texture2D texture, int x, int y, Color tint)
{
    if (!m_initialized)
        return;
    ::DrawTexture(texture, x, y, tint);
}

void CoreEngine::DrawSprite(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint)
{
    if (!m_initialized)
        return;
    DrawTexturePro(texture, source, dest, origin, rotation, tint);
}

void CoreEngine::DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color)
{
    if (!m_initialized)
        return;
    ::DrawLine(startPosX, startPosY, endPosX, endPosY, color);
}

void CoreEngine::DrawText(const char *text, int posX, int posY, int fontSize, Color color)
{
    if (!m_initialized)
        return;
    ::DrawText(text, posX, posY, fontSize, color);
}

Texture2D CoreEngine::LoadTextureFromFile(const std::string &filename)
{
    if (!m_initialized)
    {
        std::cout << "CoreEngine: Cannot load texture - engine not initialized!" << std::endl;
        return {0};
    }

    Texture2D texture = LoadTexture(filename.c_str());

    if (texture.id == 0)
    {
        std::cout << "CoreEngine: Failed to load texture: " << filename << std::endl;
    }
    else
    {
        // Track loaded texture for cleanup
        m_loadedTextures.push_back(texture);
        std::cout << "CoreEngine: Loaded texture: " << filename << " (ID: " << texture.id << ")" << std::endl;
    }

    return texture;
}

void CoreEngine::UnloadTexture(Texture2D texture)
{
    if (!m_initialized || texture.id == 0)
        return;

    // Remove from tracking vector
    auto it = std::find_if(m_loadedTextures.begin(), m_loadedTextures.end(),
                           [texture](const Texture2D &t)
                           { return t.id == texture.id; });

    if (it != m_loadedTextures.end())
    {
        m_loadedTextures.erase(it);
    }

    ::UnloadTexture(texture);
}

RenderTexture2D CoreEngine::LoadRenderTexture(int width, int height)
{
    if (!m_initialized)
    {
        std::cout << "CoreEngine: Cannot load render texture - engine not initialized!" << std::endl;
        return {0};
    }
    return ::LoadRenderTexture(width, height);
}

void CoreEngine::UnloadRenderTexture(RenderTexture2D target)
{
    if (!m_initialized)
        return;
    ::UnloadRenderTexture(target);
}

void CoreEngine::BeginTextureMode(RenderTexture2D target)
{
    if (!m_initialized)
        return;
    ::BeginTextureMode(target);
}

void CoreEngine::EndTextureMode()
{
    if (!m_initialized)
        return;
    ::EndTextureMode();
}

int CoreEngine::GetScreenWidth() const
{
    return m_screenWidth;
}

int CoreEngine::GetScreenHeight() const
{
    return m_screenHeight;
}

void CoreEngine::AddRenderObject(const RenderObject &obj)
{
    m_renderObjects.push_back(obj);
}

void CoreEngine::ClearRenderObjects()
{
    m_renderObjects.clear();
}

void CoreEngine::RenderAllObjects()
{
    if (!m_initialized)
        return;

    for (const auto &obj : m_renderObjects)
    {
        switch (obj.type)
        {
        case RenderObject::RECTANGLE:
            DrawRectangle((int)obj.position.x, (int)obj.position.y,
                          (int)obj.size.x, (int)obj.size.y, obj.color);
            break;

        case RenderObject::CIRCLE:
            DrawCircle((int)obj.position.x, (int)obj.position.y, obj.radius, obj.color);
            break;

        case RenderObject::TEXTURE:
            DrawTexture(obj.texture, (int)obj.position.x, (int)obj.position.y, obj.tint);
            break;

        case RenderObject::SPRITE:
            DrawSprite(obj.texture, obj.sourceRect, obj.destRect, obj.origin, obj.rotation, obj.tint);
            break;
        }
    }
}

// RenderObject static factory methods
RenderObject RenderObject::CreateRectangle(float x, float y, float width, float height, Color color)
{
    RenderObject obj = {};
    obj.type = RECTANGLE;
    obj.position = {x, y};
    obj.size = {width, height};
    obj.color = color;
    return obj;
}

RenderObject RenderObject::CreateCircle(float x, float y, float radius, Color color)
{
    RenderObject obj = {};
    obj.type = CIRCLE;
    obj.position = {x, y};
    obj.radius = radius;
    obj.color = color;
    return obj;
}

RenderObject RenderObject::CreateTexture(Texture2D tex, float x, float y, Color tint)
{
    RenderObject obj = {};
    obj.type = TEXTURE;
    obj.position = {x, y};
    obj.texture = tex;
    obj.tint = tint;
    return obj;
}

RenderObject RenderObject::CreateSprite(Texture2D tex, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint)
{
    RenderObject obj = {};
    obj.type = SPRITE;
    obj.texture = tex;
    obj.sourceRect = source;
    obj.destRect = dest;
    obj.origin = origin;
    obj.rotation = rotation;
    obj.tint = tint;
    return obj;
}

// Collision detection methods
bool CoreEngine::CheckCollisionRecs(Rectangle rec1, Rectangle rec2) const
{
    if (!m_initialized)
        return false;
    return ::CheckCollisionRecs(rec1, rec2);
}

bool CoreEngine::CheckCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2) const
{
    if (!m_initialized)
        return false;
    return ::CheckCollisionCircles(center1, radius1, center2, radius2);
}

bool CoreEngine::CheckCollisionCircleRec(Vector2 center, float radius, Rectangle rec) const
{
    if (!m_initialized)
        return false;
    return ::CheckCollisionCircleRec(center, radius, rec);
}

bool CoreEngine::CheckCollisionPointRec(Vector2 point, Rectangle rec) const
{
    if (!m_initialized)
        return false;
    return ::CheckCollisionPointRec(point, rec);
}

bool CoreEngine::CheckCollisionPointCircle(Vector2 point, Vector2 center, float radius) const
{
    if (!m_initialized)
        return false;
    return ::CheckCollisionPointCircle(point, center, radius);
}
