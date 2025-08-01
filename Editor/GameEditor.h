#pragma once

#include <rlImGui.h>
#include <raylib.h>
#include <imgui.h>
#include <tinyfiledialogs.h>

#include "GameEditorTheme.h"

void GameEditorInit(int width, int hight,const char* title)
{	
	InitWindow(width, hight, title);
	rlImGuiSetup(true);

	// Load a font
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	rlImGuiReloadFonts();

	SetEngineTheme();
}

void GameEditorClose()
{
	rlImGuiShutdown();
	CloseWindow();
}