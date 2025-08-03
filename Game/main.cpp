#include "GameEditor.h"

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    GameEditor editor(800, 600, "2D Game Engine with Game Logic");
    MaximizeWindow();
    editor.Run();
    return 0;
}