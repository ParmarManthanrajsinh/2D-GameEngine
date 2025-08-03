#include "GameEditor.h"

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    GameEditor editor(1280, 720, "2D Game Engine with Game Logic");
    editor.Run();
    return 0;
}