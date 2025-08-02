#include "GameEditor.h"

int main()
{
    // Initialize the game editor with integrated game logic
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    GameEditor editor(1280, 720, "2D Game Engine with Game Logic");
    editor.Run();
    return 0;
}
