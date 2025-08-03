#include "CoreEngine.h"

// CoreEngine Implementation
CoreEngine::CoreEngine()
    : bm_Initialized(false), m_ScreenWidth(800), m_ScreenHeight(600),           m_windowTitle("2D Game Engine")
{
}

CoreEngine::~CoreEngine()
{
    if (bm_Initialized)
    {
        Shutdown();
    }
}

bool CoreEngine::b_Initialize(int width, int height, const std::string &title)
{
    if (bm_Initialized)
    {
        std::cout << "CoreEngine: Already initialized!" << std::endl;
        return false;
    }

    m_ScreenWidth = width;
    m_ScreenHeight = height;
    m_windowTitle = title;

    // Initialize raylib window
    InitWindow(width, height, title.c_str());

    if (!IsWindowReady())
    {
        std::cout << "CoreEngine: Failed to initialize window!" << std::endl;
        return false;
    }

    ::SetTargetFPS(60);

    bm_Initialized = true;

    std::cout << "CoreEngine: Initialized successfully" << std::endl;
    std::cout << "  - Window: " << width << "x" << height << " '" << title << "'" << std::endl;
    std::cout << "  - Raylib Version: " << RAYLIB_VERSION << std::endl;

    return true;
}

void CoreEngine::Shutdown()
{
    if (!bm_Initialized)
    {
        return;
    }

    // Unload all tracked textures
    for (auto &texture : m_LoadedTextures)
    {
        UnloadTexture(texture);
    }
    m_LoadedTextures.clear();

    // Clear render objects
    ClearRenderObjects();

    // Close raylib window
    CloseWindow();

    bm_Initialized = false;
    std::cout << "CoreEngine: Shutdown complete" << std::endl;
}

bool CoreEngine::b_ShouldClose() const
{
    return WindowShouldClose();
}

void CoreEngine::BeginFrame()
{
    if (!bm_Initialized)
    {
        return;
    }
    BeginDrawing();
}

void CoreEngine::EndFrame()
{
    if (!bm_Initialized)
    {
        return;
    }
    EndDrawing();
}

void CoreEngine::ClearScreen(Color color)
{
    if (!bm_Initialized)
    {
        return;
    }
    ClearBackground(color);
}

void CoreEngine::DrawRectangle(int x, int y, int width, int height, Color color)
{
    if (!bm_Initialized)
    {
        return;
    }
    ::DrawRectangle(x, y, width, height, color);
}

void CoreEngine::DrawCircle(int centerX, int centerY, float radius, Color color)
{
    if (!bm_Initialized)
    {
        return;
    }
    ::DrawCircle(centerX, centerY, radius, color);
}

void CoreEngine::DrawTexture(Texture2D texture, int x, int y, Color tint)
{
    if (!bm_Initialized)
    {
        return;
    }
    ::DrawTexture(texture, x, y, tint);
}

void CoreEngine::DrawSprite(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint)
{
    if (!bm_Initialized)
    {
        return;
    }
    DrawTexturePro(texture, source, dest, origin, rotation, tint);
}

void CoreEngine::DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color)
{
    if (!bm_Initialized)
    {
        return;
    }
    ::DrawLine(startPosX, startPosY, endPosX, endPosY, color);
}

void CoreEngine::DrawText(const char *text, int posX, int posY, int fontSize, Color color)
{
    if (!bm_Initialized)
    {
        return;
    }
    ::DrawText(text, posX, posY, fontSize, color);
}

Texture2D CoreEngine::LoadTextureFromFile(const std::string &filename)
{
    if (!bm_Initialized)
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
        m_LoadedTextures.push_back(texture);
        std::cout << "CoreEngine: Loaded texture: " << filename << " (ID: " << texture.id << ")" << std::endl;
    }

    return texture;
}

void CoreEngine::UnloadTexture(Texture2D texture)
{
    if (!bm_Initialized || texture.id == 0)
    {
        return;
    }
    
    // Remove from tracking vector
    auto it = std::find_if(m_LoadedTextures.begin(), m_LoadedTextures.end(),
                           [texture](const Texture2D &t)
                           { return t.id == texture.id; });

    if (it != m_LoadedTextures.end())
    {
        m_LoadedTextures.erase(it);
    }

    ::UnloadTexture(texture);
}

RenderTexture2D CoreEngine::LoadRenderTexture(int width, int height)
{
    if (!bm_Initialized)
    {
        std::cout << "CoreEngine: Cannot load render texture - engine not initialized!" << std::endl;
        return {0};
    }
    return ::LoadRenderTexture(width, height);
}

void CoreEngine::UnloadRenderTexture(RenderTexture2D target)
{
    if (!bm_Initialized)
    {
        return;
    }
    ::UnloadRenderTexture(target);
}

void CoreEngine::BeginTextureMode(RenderTexture2D target)
{
    if (!bm_Initialized)
    {
        return;
    }
    ::BeginTextureMode(target);
}

void CoreEngine::EndTextureMode()
{
    if (!bm_Initialized)
    {
        return;
    }
    ::EndTextureMode();
}

int CoreEngine::GetScreenWidth() const
{
    return m_ScreenWidth;
}

int CoreEngine::GetScreenHeight() const
{
    return m_ScreenHeight;
}

void CoreEngine::AddRenderObject(const t_RenderObject &obj)
{
    m_RenderObjects.push_back(obj);
}

void CoreEngine::ClearRenderObjects()
{
    m_RenderObjects.clear();
}

void CoreEngine::RenderAllObjects()
{
    if (!bm_Initialized)
    {
        return;
    }
    for (const auto &obj : m_RenderObjects)
    {
        switch (obj.type)
        {
        case t_RenderObject::RECTANGLE:
            DrawRectangle((int)obj.position.x, (int)obj.position.y,
                          (int)obj.size.x, (int)obj.size.y, obj.color);
            break;

        case t_RenderObject::CIRCLE:
            DrawCircle((int)obj.position.x, (int)obj.position.y, obj.radius, obj.color);
            break;

        case t_RenderObject::TEXTURE:
            DrawTexture(obj.texture, (int)obj.position.x, (int)obj.position.y, obj.tint);
            break;

        case t_RenderObject::SPRITE:
            DrawSprite(obj.texture, obj.source_rect, obj.dest_rect, obj.origin, obj.rotation, obj.tint);
            break;
        }
    }
}

// RenderObject static factory methods
t_RenderObject t_RenderObject::sf_CreateRectangle(float x, float y, float width, float height, Color color)
{
    t_RenderObject obj = {};
    obj.type = RECTANGLE;
    obj.position = {x, y};
    obj.size = {width, height};
    obj.color = color;
    return obj;
}

t_RenderObject t_RenderObject::sf_CreateCircle(float x, float y, float radius, Color color)
{
    t_RenderObject obj = {};
    obj.type = CIRCLE;
    obj.position = {x, y};
    obj.radius = radius;
    obj.color = color;
    return obj;
}

t_RenderObject t_RenderObject::sf_CreateTexture(Texture2D tex, float x, float y, Color tint)
{
    t_RenderObject obj = {};
    obj.type = TEXTURE;
    obj.position = {x, y};
    obj.texture = tex;
    obj.tint = tint;
    return obj;
}

t_RenderObject t_RenderObject::sf_CreateSprite(Texture2D tex, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint)
{
    t_RenderObject obj = {};
    obj.type = SPRITE;
    obj.texture = tex;
    obj.source_rect = source;
    obj.dest_rect = dest;
    obj.origin = origin;
    obj.rotation = rotation;
    obj.tint = tint;
    return obj;
}

// Collision detection methods
bool CoreEngine::b_CheckCollisionRecs(Rectangle rec1, Rectangle rec2) const
{
    if (!bm_Initialized)
    {
        return false;
    }
    return ::CheckCollisionRecs(rec1, rec2);
}

bool CoreEngine::b_CheckCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2) const
{
    if (!bm_Initialized)
    {    
        return false;
    }
    return ::CheckCollisionCircles(center1, radius1, center2, radius2);
}

bool CoreEngine::b_CheckCollisionCircleRec(Vector2 center, float radius, Rectangle rec) const
{
    if (!bm_Initialized) 
    {
        return false;
    }
    return ::CheckCollisionCircleRec(center, radius, rec);
}

bool CoreEngine::b_CheckCollisionPointRec(Vector2 point, Rectangle rec) const
{
    if (!bm_Initialized) 
    {
        return false;
    }
    return ::CheckCollisionPointRec(point, rec);
}

bool CoreEngine::b_CheckCollisionPointCircle(Vector2 point, Vector2 center, float radius) const
{
    if (!bm_Initialized)
    {
        return false;
    }
    return ::CheckCollisionPointCircle(point, center, radius);
}
