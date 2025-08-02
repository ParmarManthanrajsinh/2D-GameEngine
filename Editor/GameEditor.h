#pragma once

#include <rlImGui.h>
#include <raylib.h>
#include <imgui.h>
#include <tinyfiledialogs.h>
#include <memory>

#include "GameEditorTheme.h"
#include "GameEditorLayout.h"
#include "CoreEngine.h"

class GameEditor {
public:
    GameEditor(int width, int height, const char* title);
    ~GameEditor();

    void Run();

private:
    void Init(int width, int height, const char* title);
    void Close();

    // Forward declaration for pImpl pattern
    class Impl;
    std::unique_ptr<Impl> pImpl;
};
