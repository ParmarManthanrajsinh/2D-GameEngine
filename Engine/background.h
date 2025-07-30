#pragma once

#include <raylib.h>

class Background
{
private:
    Texture2D texture;

public:
    Background(const char *filePath);
    ~Background();
    void Draw() const;
};