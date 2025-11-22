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

[Window][Scene]
Pos=0,0
Size=972,720
Collapsed=0
DockId=0x00000001,0

[Window][Map Selection]
Pos=974,0
Size=306,720
Collapsed=0
DockId=0x00000002,0

[Docking][Data]
DockSpace     ID=0x08BD597D Window=0x1BBC0F80 Pos=0,0 Size=1280,720 Split=Y Selected=0xE601B12F
  DockNode    ID=0x00000003 Parent=0x08BD597D SizeRef=1280,520 Split=X Selected=0xE601B12F
    DockNode  ID=0x00000001 Parent=0x00000003 SizeRef=972,720 CentralNode=1 Selected=0xE601B12F
    DockNode  ID=0x00000002 Parent=0x00000003 SizeRef=306,720 Selected=0x9D14B58E
  DockNode    ID=0x00000004 Parent=0x08BD597D SizeRef=1280,198 Selected=0x9C2B5678


)";

inline void LoadEditorDefaultIni()
{
    ImGui::LoadIniSettingsFromMemory(sc_EDITOR_DEFAULT_INI, strlen(sc_EDITOR_DEFAULT_INI));
}
