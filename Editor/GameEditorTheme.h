#pragma once
#include <string>

inline void SetEngineTheme(const std::string& path = "Assets/Roboto-Regular.ttf")
{
	// Load a custom font with better size for engine UI
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear();
	io.Fonts->AddFontFromFileTTF(path.c_str(), 16.0f);
	rlImGuiReloadFonts();

	ImGuiStyle& style = ImGui::GetStyle();

	// UE5 Color Palette
	const ImVec4 ue5_dark_bg = ImVec4(0.08f, 0.08f, 0.08f, 1.0f);           // Main dark background
	const ImVec4 ue5_panel_bg = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);         // Panel backgrounds
	const ImVec4 ue5_widget_bg = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);        // Widget backgrounds
	const ImVec4 ue5_widget_hover = ImVec4(0.22f, 0.22f, 0.22f, 1.0f);     // Widget hover
	const ImVec4 ue5_widget_active = ImVec4(0.28f, 0.28f, 0.28f, 1.0f);    // Widget active
	const ImVec4 ue5_accent_blue = ImVec4(0.10f, 0.40f, 0.75f, 1.0f);      // UE5 signature blue
	const ImVec4 ue5_accent_blue_hover = ImVec4(0.15f, 0.50f, 0.85f, 1.0f); // Brighter blue
	const ImVec4 ue5_accent_blue_active = ImVec4(0.05f, 0.30f, 0.65f, 1.0f); // Darker blue
	const ImVec4 ue5_text_primary = ImVec4(0.92f, 0.92f, 0.92f, 1.0f);     // Primary text
	const ImVec4 ue5_text_secondary = ImVec4(0.70f, 0.70f, 0.70f, 1.0f);   // Secondary text
	const ImVec4 ue5_text_disabled = ImVec4(0.45f, 0.45f, 0.45f, 1.0f);    // Disabled text
	const ImVec4 ue5_border = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);           // Borders
	const ImVec4 ue5_separator = ImVec4(0.30f, 0.30f, 0.30f, 1.0f);        // Separators

	// Window and background colors
	style.Colors[ImGuiCol_WindowBg] = ue5_dark_bg;
	style.Colors[ImGuiCol_ChildBg] = ue5_panel_bg;
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.98f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.0f);

	// Frame backgrounds (input fields, etc.)
	style.Colors[ImGuiCol_FrameBg] = ue5_widget_bg;
	style.Colors[ImGuiCol_FrameBgHovered] = ue5_widget_hover;
	style.Colors[ImGuiCol_FrameBgActive] = ue5_widget_active;

	// Button styling with UE5 blue accents
	style.Colors[ImGuiCol_Button] = ue5_widget_bg;
	style.Colors[ImGuiCol_ButtonHovered] = ue5_accent_blue_hover;
	style.Colors[ImGuiCol_ButtonActive] = ue5_accent_blue_active;

	// Headers (collapsible sections, column headers)
	style.Colors[ImGuiCol_Header] = ue5_widget_bg;
	style.Colors[ImGuiCol_HeaderHovered] = ue5_accent_blue_hover;
	style.Colors[ImGuiCol_HeaderActive] = ue5_accent_blue;

	// Title bar styling
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ue5_accent_blue;
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.04f, 0.04f, 0.04f, 0.8f);

	// Scrollbars with UE5 styling
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ue5_widget_bg;
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ue5_widget_hover;
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ue5_accent_blue;

	// Check marks and radio buttons
	style.Colors[ImGuiCol_CheckMark] = ue5_accent_blue;

	// Sliders with blue accent
	style.Colors[ImGuiCol_SliderGrab] = ue5_accent_blue;
	style.Colors[ImGuiCol_SliderGrabActive] = ue5_accent_blue_hover;

	// Text colors
	style.Colors[ImGuiCol_Text] = ue5_text_primary;
	style.Colors[ImGuiCol_TextDisabled] = ue5_text_disabled;
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.10f, 0.40f, 0.75f, 0.35f);

	// Borders and separators
	style.Colors[ImGuiCol_Border] = ue5_border;
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_Separator] = ue5_separator;
	style.Colors[ImGuiCol_SeparatorHovered] = ue5_accent_blue_hover;
	style.Colors[ImGuiCol_SeparatorActive] = ue5_accent_blue;

	// Tab styling (like UE5's dockable panels)
	style.Colors[ImGuiCol_Tab] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ue5_accent_blue_hover;
	style.Colors[ImGuiCol_TabActive] = ue5_accent_blue;
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.08f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);

	// Table colors (for content browser, outliner, etc.)
	style.Colors[ImGuiCol_TableHeaderBg] = ue5_panel_bg;
	style.Colors[ImGuiCol_TableBorderStrong] = ue5_border;
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.03f);

	// Drag and drop
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.10f, 0.40f, 0.75f, 0.90f);

	// Navigation and selection
	style.Colors[ImGuiCol_NavHighlight] = ue5_accent_blue;
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);

	// Resize grip (corner handle)
	style.Colors[ImGuiCol_ResizeGrip] = ue5_widget_bg;
	style.Colors[ImGuiCol_ResizeGripHovered] = ue5_accent_blue_hover;
	style.Colors[ImGuiCol_ResizeGripActive] = ue5_accent_blue;

	// UE5-style rounded corners and modern look
	style.WindowRounding = 4.0f;
	style.ChildRounding = 4.0f;
	style.FrameRounding = 3.0f;
	style.PopupRounding = 4.0f;
	style.ScrollbarRounding = 4.0f;
	style.GrabRounding = 3.0f;
	style.LogSliderDeadzone = 4.0f;
	style.TabRounding = 3.0f;

	// UE5-style spacing and padding
	style.WindowPadding = ImVec2(12.0f, 12.0f);
	style.FramePadding = ImVec2(10.0f, 6.0f);
	style.CellPadding = ImVec2(8.0f, 6.0f);
	style.ItemSpacing = ImVec2(8.0f, 8.0f);
	style.ItemInnerSpacing = ImVec2(6.0f, 6.0f);
	style.TouchExtraPadding = ImVec2(0.0f, 0.0f);
	style.IndentSpacing = 24.0f;
	style.ScrollbarSize = 16.0f;
	style.GrabMinSize = 14.0f;

	// Professional border sizing
	style.WindowBorderSize = 1.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupBorderSize = 1.0f;
	style.FrameBorderSize = 0.0f;
	style.TabBorderSize = 0.0f;

	// UE5-style window behavior
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);  // Centered title
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.5f);

	// High-quality anti-aliasing for professional look
	style.AntiAliasedLines = true;
	style.AntiAliasedLinesUseTex = true;
	style.AntiAliasedFill = true;

	// Additional UE5-inspired tweaks
	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.DisplayWindowPadding = ImVec2(19.0f, 19.0f);
	style.DisplaySafeAreaPadding = ImVec2(3.0f, 3.0f);
}