#include "GameEditor.h"

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    GameEditor editor(1280, 720, "Simple Game Engine");
    editor.Run();
    return 0;
}
