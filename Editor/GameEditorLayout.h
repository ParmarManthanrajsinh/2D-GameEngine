#include<imgui.h>
#include<cstring>

// Optional: disable saving to file
// ImGui::GetIO().IniFilename = nullptr;

// Default docking layout for the editor, loaded on startup
static const char* sc_EDITOR_DEFAULT_INI = 
R"(
[Window][WindowOverViewport_11111111]
Pos=0,0
Size=1280,720
Collapsed=0

[Window][Debug##Default]
Pos=60,60
Size=400,400
Collapsed=0

[Window][File Explorer]
Pos=753,427
Size=234,198
Collapsed=0

[Window][Scene]
Pos=0,0
Size=1280,720
Collapsed=0
DockId=0x08BD597D,0

[Docking][Data]
DockSpace ID=0x08BD597D Window=0x1BBC0F80 Pos=0,0 Size=1280,720 CentralNode=1 Selected=0xE601B12F
)";

inline void LoadEditorDefaultIni()
{
    ImGui::LoadIniSettingsFromMemory(sc_EDITOR_DEFAULT_INI, strlen(sc_EDITOR_DEFAULT_INI));
}
