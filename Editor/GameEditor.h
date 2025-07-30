#pragma once

#include <raylib.h>
#include <rlImGui.h>
#include <imgui.h>
#include <tinyfiledialogs.h>

void GameEditorInit(int width, int hight,const char* title)
{	
	InitWindow(width, hight, title);
	rlImGuiSetup(true);

	// Load a font
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear();
	io.Fonts->AddFontFromFileTTF("Assets/Roboto-Regular.ttf", 24.0f);
	rlImGuiReloadFonts();
}

void GameEditorClose()
{
	rlImGuiShutdown();
	CloseWindow();
}