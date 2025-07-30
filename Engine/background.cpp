#include "background.h"

Background::Background(const char *filePath)
{
    Image image = LoadImage(filePath);
    if (image.data == nullptr) {
        TraceLog(LOG_ERROR, "Failed to load image: %s", filePath);
        return;
    }
    texture = LoadTextureFromImage(image);
    UnloadImage(image); // Unload image after creating texture
}

Background::~Background()
{
    UnloadTexture(texture); // Unload texture when Background object is destroyed
}

void Background::Draw() const
{
    DrawTexture(texture, 0, 0, WHITE); // Draw the texture at position (0, 0)
}