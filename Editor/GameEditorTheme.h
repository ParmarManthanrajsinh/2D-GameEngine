#pragma once
#include <string>

inline void SetEngineTheme(const std::string& path = "Assets/Roboto-Regular.ttf")
{
	// Load optimized font for game engine UI with multiple sizes
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear();
	
	// Load primary font with crisp rendering
	ImFontConfig font_config;
	font_config.OversampleH = 3;
	font_config.OversampleV = 2;
	font_config.PixelSnapH = true;
	io.Fonts->AddFontFromFileTTF(path.c_str(), 16.0f, &font_config);
	
	// Add icon font for enhanced UI (if available)
	// io.Fonts->AddFontFromMemoryCompressedTTF(fa_solid_900_compressed_data, fa_solid_900_compressed_size, 14.0f);
	
	rlImGuiReloadFonts();

	ImGuiStyle& style = ImGui::GetStyle();

	// === ULTRA-PREMIUM GAME ENGINE PALETTE - NEXT GENERATION ===
	// Base Layer - Deep Space Inspired
	const ImVec4 void_black = ImVec4(0.02f, 0.02f, 0.04f, 1.0f);              // Ultra-deep void
	const ImVec4 midnight_black = ImVec4(0.05f, 0.05f, 0.08f, 1.0f);          // Deep midnight base
	const ImVec4 charcoal_base = ImVec4(0.10f, 0.10f, 0.14f, 1.0f);           // Rich charcoal panels
	const ImVec4 slate_surface = ImVec4(0.15f, 0.15f, 0.20f, 1.0f);           // Elevated surfaces
	const ImVec4 obsidian_widget = ImVec4(0.20f, 0.18f, 0.23f, 1.0f);         // Widget backgrounds
	
	// Premium Crimson Accent System - Cinema Grade
	const ImVec4 crimson_primary = ImVec4(0.95f, 0.20f, 0.20f, 1.0f);         // Vibrant primary
	const ImVec4 crimson_bright = ImVec4(1.00f, 0.35f, 0.35f, 1.0f);          // Brilliant hover
	const ImVec4 crimson_intense = ImVec4(0.85f, 0.10f, 0.10f, 1.0f);         // Deep active
	const ImVec4 crimson_glow = ImVec4(0.95f, 0.20f, 0.20f, 0.18f);           // Ambient glow
	const ImVec4 crimson_selection = ImVec4(0.95f, 0.20f, 0.20f, 0.28f);      // Selection overlay
	const ImVec4 crimson_subtle = ImVec4(0.75f, 0.15f, 0.15f, 0.8f);          // Subtle accent
	
	// Ultra-Smooth Hover States - Each Element Type Optimized
	const ImVec4 button_hover = ImVec4(0.32f, 0.28f, 0.30f, 1.0f);            // Warm button lift
	const ImVec4 button_hover_bright = ImVec4(0.38f, 0.32f, 0.35f, 1.0f);     // Bright button state
	const ImVec4 frame_hover = ImVec4(0.25f, 0.23f, 0.28f, 1.0f);             // Input field focus
	const ImVec4 frame_hover_active = ImVec4(0.30f, 0.26f, 0.32f, 1.0f);      // Active input state
	const ImVec4 header_hover = ImVec4(0.40f, 0.30f, 0.35f, 1.0f);            // Header engagement
	const ImVec4 tab_hover = ImVec4(0.35f, 0.28f, 0.32f, 1.0f);               // Tab preview
	const ImVec4 scrollbar_hover = ImVec4(0.42f, 0.35f, 0.40f, 1.0f);         // Scrollbar highlight
	const ImVec4 separator_hover = ImVec4(0.60f, 0.55f, 0.58f, 1.0f);         // Separator feedback
	
	// Platinum Text Hierarchy - Ultra Readability
	const ImVec4 platinum_text = ImVec4(0.99f, 0.97f, 0.99f, 1.0f);           // Primary text
	const ImVec4 silver_text = ImVec4(0.85f, 0.83f, 0.87f, 1.0f);             // Secondary text
	const ImVec4 bronze_text = ImVec4(0.68f, 0.66f, 0.70f, 1.0f);             // Tertiary text
	const ImVec4 disabled_text = ImVec4(0.45f, 0.43f, 0.47f, 1.0f);           // Disabled state
	const ImVec4 muted_text = ImVec4(0.55f, 0.53f, 0.57f, 1.0f);              // Muted information
	
	// Advanced Border & Separator System
	const ImVec4 crimson_border = ImVec4(0.50f, 0.40f, 0.43f, 1.0f);          // Accent borders
	const ImVec4 neutral_border = ImVec4(0.35f, 0.33f, 0.37f, 1.0f);          // Standard borders
	const ImVec4 subtle_border = ImVec4(0.25f, 0.23f, 0.27f, 1.0f);           // Subtle divisions
	const ImVec4 accent_separator = ImVec4(0.42f, 0.32f, 0.37f, 1.0f);        // Feature separators

	// === WINDOW AND BACKGROUND SYSTEM - ULTRA DEPTH ===
	style.Colors[ImGuiCol_WindowBg] = void_black;
	style.Colors[ImGuiCol_ChildBg] = charcoal_base;
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.12f, 0.98f);       // Enhanced popup depth
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.06f, 0.06f, 0.10f, 1.0f);      // Deep menu bar

	// === FRAME SYSTEM - ULTRA-RESPONSIVE INPUT FIELDS ===
	style.Colors[ImGuiCol_FrameBg] = obsidian_widget;
	style.Colors[ImGuiCol_FrameBgHovered] = frame_hover;                        // Sophisticated hover
	style.Colors[ImGuiCol_FrameBgActive] = frame_hover_active;                  // Enhanced active state

	// === BUTTON SYSTEM - CINEMATIC HOVER PROGRESSION ===
	style.Colors[ImGuiCol_Button] = slate_surface;
	style.Colors[ImGuiCol_ButtonHovered] = button_hover;                        // Premium button lift
	style.Colors[ImGuiCol_ButtonActive] = crimson_intense;                      // Dramatic active state

	// === HEADER SYSTEM - ELEGANT COLLAPSIBLE SECTIONS ===
	style.Colors[ImGuiCol_Header] = obsidian_widget;
	style.Colors[ImGuiCol_HeaderHovered] = header_hover;                        // Refined header engagement
	style.Colors[ImGuiCol_HeaderActive] = crimson_primary;

	// === TITLE BAR SYSTEM - PREMIUM WINDOW HEADERS ===
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.08f, 1.0f);        // Deep title background
	style.Colors[ImGuiCol_TitleBgActive] = crimson_primary;                     // Active window highlight
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08f, 0.08f, 0.12f, 0.85f);

	// === THIN SCROLLBAR SYSTEM - Elegant & Minimal ===
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.03f, 0.03f, 0.06f, 0.90f);   // Subtle thin track
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.35f, 0.33f, 0.37f, 0.80f); // Visible thin grab
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.45f, 0.38f, 0.42f, 0.90f); // Elegant hover
	style.Colors[ImGuiCol_ScrollbarGrabActive] = crimson_bright;                // Bright active state

	// === INTERACTIVE ELEMENTS - PRECISION CONTROLS ===
	style.Colors[ImGuiCol_CheckMark] = crimson_primary;                         // Checkbox marks
	style.Colors[ImGuiCol_SliderGrab] = crimson_primary;                        // Slider handles
	style.Colors[ImGuiCol_SliderGrabActive] = crimson_bright;                   // Active slider

	// === TEXT SYSTEM - ULTRA-PREMIUM READABILITY ===
	style.Colors[ImGuiCol_Text] = platinum_text;                               // Primary text
	style.Colors[ImGuiCol_TextDisabled] = disabled_text;                        // Disabled text
	style.Colors[ImGuiCol_TextSelectedBg] = crimson_selection;                  // Text selection

	// === BORDER AND SEPARATOR SYSTEM - REFINED DIVISIONS ===
	style.Colors[ImGuiCol_Border] = crimson_border;                             // Accent borders
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.40f);  // Enhanced shadow
	style.Colors[ImGuiCol_Separator] = accent_separator;                        // Feature separators
	style.Colors[ImGuiCol_SeparatorHovered] = separator_hover;                  // Interactive separator
	style.Colors[ImGuiCol_SeparatorActive] = crimson_primary;                   // Active separator

	// === TAB SYSTEM - ULTRA-ADVANCED DOCKING INTERFACE ===
	style.Colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.12f, 0.16f, 1.0f);            // Inactive tab base
	style.Colors[ImGuiCol_TabHovered] = tab_hover;                              // Premium tab preview
	style.Colors[ImGuiCol_TabActive] = crimson_primary;                         // Active tab highlight
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.12f, 1.0f);   // Unfocused background
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.15f, 0.20f, 1.0f); // Unfocused active

	// === TABLE SYSTEM - PREMIUM DATA DISPLAY ===
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.18f, 0.16f, 0.20f, 1.0f);  // Enhanced header
	style.Colors[ImGuiCol_TableBorderStrong] = crimson_border;                  // Strong table borders
	style.Colors[ImGuiCol_TableBorderLight] = neutral_border;                   // Light table borders
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);    // Transparent rows
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.98f, 0.92f, 0.94f, 0.03f); // Subtle alternating

	// === DRAG AND DROP SYSTEM - ENHANCED VISUAL FEEDBACK ===
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.95f, 0.20f, 0.20f, 0.90f); // Enhanced drop target

	// === NAVIGATION SYSTEM - ULTRA-RESPONSIVE ===
	style.Colors[ImGuiCol_NavHighlight] = crimson_bright;                       // Navigation highlight
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.98f, 0.92f, 0.94f, 0.80f); // Window highlight
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.12f, 0.08f, 0.10f, 0.30f); // Background dimming
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.04f, 0.02f, 0.03f, 0.80f); // Modal dimming

	// === RESIZE GRIP SYSTEM - PREMIUM WINDOW CONTROLS ===
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.25f, 0.23f, 0.27f, 1.0f);     // Subtle grip
	style.Colors[ImGuiCol_ResizeGripHovered] = crimson_bright;                  // Hover feedback
	style.Colors[ImGuiCol_ResizeGripActive] = crimson_primary;                  // Active resize

	// === PLOTTING SYSTEM - ENHANCED DEBUG & PROFILER ===
	style.Colors[ImGuiCol_PlotLines] = silver_text;                             // Plot line color
	style.Colors[ImGuiCol_PlotLinesHovered] = crimson_bright;                   // Hovered plot lines
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.70f, 0.18f, 0.18f, 1.0f);  // Histogram bars
	style.Colors[ImGuiCol_PlotHistogramHovered] = crimson_bright;               // Hovered histogram

	// === REFINED GEOMETRY - Thin & Elegant ===
	style.WindowRounding = 2.0f;                                             // Subtle window corners
	style.ChildRounding = 1.5f;                                              // Minimal child rounding
	style.FrameRounding = 1.5f;                                              // Thin frame elegance
	style.PopupRounding = 2.5f;                                              // Refined popups
	style.ScrollbarRounding = 6.0f;                                          // Thin smooth scrollbars
	style.GrabRounding = 1.5f;                                               // Minimal grab handles
	style.LogSliderDeadzone = 3.0f;                                          // Precise slider control
	style.TabRounding = 1.5f;                                                // Subtle tab corners

	// === ULTRA-PREMIUM SPACING SYSTEM - Ergonomic Excellence ===
	style.WindowPadding = ImVec2(10.0f, 10.0f);                              // Refined window padding
	style.FramePadding = ImVec2(8.0f, 4.0f);                                 // Thinner frame padding
	style.CellPadding = ImVec2(6.0f, 4.0f);                                  // Compact cell spacing
	style.ItemSpacing = ImVec2(6.0f, 6.0f);                                  // Tighter item spacing
	style.ItemInnerSpacing = ImVec2(4.0f, 3.0f);                             // Refined inner spacing
	style.TouchExtraPadding = ImVec2(0.0f, 0.0f);                            // Clean touch interface
	style.IndentSpacing = 20.0f;                                             // Compact indentation
	style.ScrollbarSize = 10.0f;                                             // Thin scrollbar size
	style.GrabMinSize = 10.0f;                                               // Thin grab targets

	// === THIN BORDER SYSTEM - Minimalist Precision ===
	style.WindowBorderSize = 0.8f;                                           // Thin window borders
	style.ChildBorderSize = 0.6f;                                            // Ultra-thin child borders
	style.PopupBorderSize = 0.8f;                                            // Thin popup borders
	style.FrameBorderSize = 0.6f;                                            // Minimal frame borders
	style.TabBorderSize = 0.0f;                                              // Borderless tabs

	// === ALIGNMENT SYSTEM - GAME ENGINE OPTIMIZED ===
	style.WindowTitleAlign = ImVec2(0.02f, 0.5f);                            // Refined title positioning
	style.WindowMenuButtonPosition = ImGuiDir_Left;                           // Menu button placement
	style.ColorButtonPosition = ImGuiDir_Right;                               // Color picker position
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);                              // Centered button text
	style.SelectableTextAlign = ImVec2(0.0f, 0.5f);                          // Selectable alignment

	// === ULTRA-HIGH QUALITY RENDERING - CINEMA GRADE ===
	style.AntiAliasedLines = true;                                            // Smooth line rendering
	style.AntiAliasedLinesUseTex = true;                                      // Texture-based AA
	style.AntiAliasedFill = true;                                             // Smooth fill rendering

	// === GAME ENGINE OPTIMIZATION SETTINGS - REFINED ===
	style.Alpha = 1.0f;                                                       // Full opacity
	style.DisabledAlpha = 0.40f;                                              // Enhanced disabled state
	style.WindowMinSize = ImVec2(180.0f, 120.0f);                            // Compact minimum size
	style.DisplayWindowPadding = ImVec2(18.0f, 18.0f);                       // Refined display padding
	style.DisplaySafeAreaPadding = ImVec2(3.0f, 3.0f);                       // Minimal safe area

	// === ADVANCED RENDERING OPTIMIZATION - ULTRA SMOOTH ===
	style.CircleTessellationMaxError = 0.20f;                                 // Ultra-smooth circles
	style.CurveTessellationTol = 1.0f;                                        // Perfect curve quality

	// === ULTRA-PREMIUM HOVER EFFECT SETTINGS - BUG-FREE ===
	// Optimized timing for ultra-responsive, glitch-free hover states
	style.HoverStationaryDelay = 0.12f;                                       // Lightning-fast response
	style.HoverDelayShort = 0.05f;                                            // Instant visual feedback
	style.HoverDelayNormal = 0.18f;                                           // Smooth tooltip timing
	
	// === ADDITIONAL ULTRA-PREMIUM SETTINGS ===
	// These ensure perfect hover state transitions without conflicts
	io.MouseDoubleClickTime = 0.30f;                                          // Precise double-click
	io.MouseDoubleClickMaxDist = 6.0f;                                        // Click precision
	io.KeyRepeatDelay = 0.275f;                                               // Responsive key repeat
	io.KeyRepeatRate = 0.050f;                                                // Smooth key repetition
	
	// === ULTRA-LEVEL HOVER ENHANCEMENT SYSTEM ===
	// Revolutionary hover detection and response system
	io.MouseDragThreshold = 3.0f;                                             // Precise drag detection
	io.ConfigWindowsMoveFromTitleBarOnly = true;                              // Clean window handling
	io.ConfigWindowsResizeFromEdges = true;                                   // Edge resize capability
	
	// === GAME ENGINE SPECIFIC OPTIMIZATIONS ===
	// Tailored for maximum performance in game development
	io.ConfigInputTrickleEventQueue = true;                                   // Smooth input handling
	io.ConfigInputTextCursorBlink = true;                                     // Enhanced text editing
	io.ConfigInputTextEnterKeepActive = false;                                // Clean form behavior
	
	// === ULTRA-PREMIUM VISUAL POLISH ===
	// Final touches for maximum visual appeal
	io.ConfigDragClickToInputText = false;                                    // Prevent accidental edits
	io.ConfigMacOSXBehaviors = false;                                         // Windows-optimized behavior
}