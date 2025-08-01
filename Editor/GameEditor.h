#pragma once

#include <rlImGui.h>
#include <raylib.h>
#include <imgui.h>
#include <tinyfiledialogs.h>

#include "GameEditorTheme.h"
#include "GameEditorLayout.h"

void GameEditorInit(int width, int hight,const char* title)
{	
	InitWindow(width, hight, title);
	rlImGuiSetup(true);

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	rlImGuiReloadFonts();

	SetEngineTheme();
	LoadEditorDefaultIni();
}

void GameEditorClose()
{
	rlImGuiShutdown();
	CloseWindow();
}