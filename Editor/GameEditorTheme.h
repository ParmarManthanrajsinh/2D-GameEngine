#pragma once
#include <string>

inline void SetEngineTheme(const std::string& path = "Assets/Roboto-Regular.ttf")
{
	// Load optimized font for game engine UI
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear();
	io.Fonts->AddFontFromFileTTF(path.c_str(), 15.0f);
	rlImGuiReloadFonts();

	ImGuiStyle& style = ImGui::GetStyle();

	// Ultra-Premium Game Engine Red Palette - Completely Redesigned
	const ImVec4 midnight_black = ImVec4(0.04f, 0.04f, 0.06f, 1.0f);          // Deep midnight base
	const ImVec4 charcoal_base = ImVec4(0.09f, 0.09f, 0.12f, 1.0f);           // Rich charcoal panels
	const ImVec4 slate_surface = ImVec4(0.14f, 0.14f, 0.18f, 1.0f);           // Elevated surfaces
	const ImVec4 obsidian_widget = ImVec4(0.18f, 0.16f, 0.20f, 1.0f);         // Widget backgrounds

	// Sophisticated Red Accent System
	const ImVec4 crimson_primary = ImVec4(0.92f, 0.18f, 0.18f, 1.0f);         // Vibrant crimson primary
	const ImVec4 crimson_hover = ImVec4(1.00f, 0.28f, 0.28f, 1.0f);           // Luminous hover state
	const ImVec4 crimson_active = ImVec4(0.82f, 0.08f, 0.08f, 1.0f);          // Deep active state
	const ImVec4 crimson_glow = ImVec4(0.92f, 0.18f, 0.18f, 0.15f);           // Subtle glow effect
	const ImVec4 crimson_selection = ImVec4(0.92f, 0.18f, 0.18f, 0.25f);      // Selection overlay

	// Advanced Hover States - Unique for each element type
	const ImVec4 button_hover = ImVec4(0.28f, 0.24f, 0.26f, 1.0f);            // Warm button hover
	const ImVec4 frame_hover = ImVec4(0.22f, 0.20f, 0.24f, 1.0f);             // Input field hover
	const ImVec4 header_hover = ImVec4(0.35f, 0.25f, 0.28f, 1.0f);            // Header hover with red tint
	const ImVec4 tab_hover = ImVec4(0.30f, 0.22f, 0.26f, 1.0f);               // Tab hover state

	// Premium Text System
	const ImVec4 platinum_text = ImVec4(0.98f, 0.96f, 0.98f, 1.0f);           // Ultra-high contrast
	const ImVec4 silver_text = ImVec4(0.82f, 0.80f, 0.84f, 1.0f);             // Secondary text
	const ImVec4 bronze_text = ImVec4(0.65f, 0.63f, 0.67f, 1.0f);             // Tertiary text
	const ImVec4 disabled_text = ImVec4(0.42f, 0.40f, 0.44f, 1.0f);           // Disabled state

	// Border and Separator System
	const ImVec4 crimson_border = ImVec4(0.45f, 0.35f, 0.38f, 1.0f);          // Red-tinted borders
	const ImVec4 neutral_border = ImVec4(0.32f, 0.30f, 0.34f, 1.0f);          // Neutral borders
	const ImVec4 accent_separator = ImVec4(0.38f, 0.28f, 0.32f, 1.0f);        // Accent separators

	// === WINDOW AND BACKGROUND SYSTEM ===
	style.Colors[ImGuiCol_WindowBg] = midnight_black;
	style.Colors[ImGuiCol_ChildBg] = charcoal_base;
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.11f, 0.98f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.06f, 0.06f, 0.09f, 1.0f);

	// === FRAME SYSTEM - Input Fields & Controls ===
	style.Colors[ImGuiCol_FrameBg] = obsidian_widget;
	style.Colors[ImGuiCol_FrameBgHovered] = frame_hover;                        // Unique hover for inputs
	style.Colors[ImGuiCol_FrameBgActive] = crimson_glow;                        // Active input glow

	// === BUTTON SYSTEM - Sophisticated Hover Progression ===
	style.Colors[ImGuiCol_Button] = slate_surface;
	style.Colors[ImGuiCol_ButtonHovered] = button_hover;                        // Distinctive button hover
	style.Colors[ImGuiCol_ButtonActive] = crimson_active;

	// === HEADER SYSTEM - Collapsible Sections ===
	style.Colors[ImGuiCol_Header] = obsidian_widget;
	style.Colors[ImGuiCol_HeaderHovered] = header_hover;                        // Premium header hover
	style.Colors[ImGuiCol_HeaderActive] = crimson_primary;

	// === TITLE BAR SYSTEM - Window Headers ===
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.05f, 0.05f, 0.08f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = crimson_primary;
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.07f, 0.07f, 0.10f, 0.85f);

	// === SCROLLBAR SYSTEM - Enhanced Visual Feedback ===
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.09f, 0.95f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.25f, 0.23f, 0.27f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.35f, 0.28f, 0.32f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = crimson_hover;

	// === INTERACTIVE ELEMENTS ===
	style.Colors[ImGuiCol_CheckMark] = crimson_primary;
	style.Colors[ImGuiCol_SliderGrab] = crimson_primary;
	style.Colors[ImGuiCol_SliderGrabActive] = crimson_hover;

	// === TEXT SYSTEM - Ultra-Premium Readability ===
	style.Colors[ImGuiCol_Text] = platinum_text;
	style.Colors[ImGuiCol_TextDisabled] = disabled_text;
	style.Colors[ImGuiCol_TextSelectedBg] = crimson_selection;

	// === BORDER AND SEPARATOR SYSTEM ===
	style.Colors[ImGuiCol_Border] = crimson_border;
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.35f);
	style.Colors[ImGuiCol_Separator] = accent_separator;
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.50f, 0.50f, 0.52f, 1.0f);   // Light gray separator hover
	style.Colors[ImGuiCol_SeparatorActive] = crimson_primary;

	// === TAB SYSTEM - Advanced Docking Interface ===
	style.Colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.12f, 0.15f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = tab_hover;                              // Unique tab hover
	style.Colors[ImGuiCol_TabActive] = crimson_primary;
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.09f, 0.09f, 0.12f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.13f, 0.16f, 1.0f);

	// === TABLE SYSTEM - Advanced Data Display ===
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.16f, 0.14f, 0.18f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = crimson_border;
	style.Colors[ImGuiCol_TableBorderLight] = neutral_border;
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.98f, 0.92f, 0.94f, 0.02f);

	// === DRAG AND DROP SYSTEM ===
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.92f, 0.18f, 0.18f, 0.85f);

	// === NAVIGATION SYSTEM ===
	style.Colors[ImGuiCol_NavHighlight] = crimson_primary;
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.98f, 0.92f, 0.94f, 0.75f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.12f, 0.08f, 0.10f, 0.25f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.04f, 0.02f, 0.03f, 0.75f);

	// === RESIZE GRIP SYSTEM ===
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.22f, 0.20f, 0.24f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripHovered] = crimson_hover;
	style.Colors[ImGuiCol_ResizeGripActive] = crimson_primary;

	// === PLOTTING SYSTEM - Debug & Profiler ===
	style.Colors[ImGuiCol_PlotLines] = silver_text;
	style.Colors[ImGuiCol_PlotLinesHovered] = crimson_hover;
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.65f, 0.15f, 0.15f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = crimson_hover;

	// === ULTRA-PREMIUM GEOMETRY - Subtle but Sophisticated ===
	style.WindowRounding = 2.5f;
	style.ChildRounding = 2.0f;
	style.FrameRounding = 2.0f;
	style.PopupRounding = 3.0f;
	style.ScrollbarRounding = 8.0f;        // Smooth scrollbars
	style.GrabRounding = 2.0f;
	style.LogSliderDeadzone = 3.0f;
	style.TabRounding = 2.0f;

	// === PROFESSIONAL SPACING SYSTEM ===
	style.WindowPadding = ImVec2(10.0f, 10.0f);
	style.FramePadding = ImVec2(9.0f, 5.0f);
	style.CellPadding = ImVec2(7.0f, 5.0f);
	style.ItemSpacing = ImVec2(7.0f, 7.0f);
	style.ItemInnerSpacing = ImVec2(5.0f, 4.0f);
	style.TouchExtraPadding = ImVec2(0.0f, 0.0f);
	style.IndentSpacing = 22.0f;
	style.ScrollbarSize = 15.0f;
	style.GrabMinSize = 13.0f;

	// === BORDER SYSTEM - Precision Engineering ===
	style.WindowBorderSize = 1.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupBorderSize = 1.0f;
	style.FrameBorderSize = 1.0f;
	style.TabBorderSize = 0.0f;

	// === ALIGNMENT SYSTEM - Game Engine Optimized ===
	style.WindowTitleAlign = ImVec2(0.015f, 0.5f);        // Subtle left offset
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.5f);

	// === ULTRA-HIGH QUALITY RENDERING ===
	style.AntiAliasedLines = true;
	style.AntiAliasedLinesUseTex = true;
	style.AntiAliasedFill = true;

	// === GAME ENGINE OPTIMIZATION SETTINGS ===
	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.45f;
	style.WindowMinSize = ImVec2(180.0f, 120.0f);
	style.DisplayWindowPadding = ImVec2(20.0f, 20.0f);
	style.DisplaySafeAreaPadding = ImVec2(3.0f, 3.0f);

	// === ADVANCED RENDERING OPTIMIZATION ===
	style.CircleTessellationMaxError = 0.25f;             // Smoother circles
	style.CurveTessellationTol = 1.1f;                    // Better curves

	// === ULTRA-PREMIUM HOVER EFFECT SETTINGS ===
	// These ensure hover states never conflict or override each other
	style.HoverStationaryDelay = 0.15f;                   // Quick hover response
	style.HoverDelayShort = 0.08f;                        // Instant feedback
	style.HoverDelayNormal = 0.25f;                       // Tooltips
}