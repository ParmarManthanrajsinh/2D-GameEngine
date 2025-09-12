﻿#pragma once
#include <string>
#include <imgui.h>
#include <rlImGui.h>

inline void SetEngineTheme(const std::string& path = "Assets/Roboto-Regular.ttf")
{
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();

    ImFontConfig font_config;
    font_config.OversampleH = 2;
    font_config.OversampleV = 1;
    font_config.PixelSnapH = true;
    font_config.RasterizerMultiply = 1.15f;

    io.Fonts->AddFontFromFileTTF(path.c_str(), 16.0f, &font_config);
    io.Fonts->AddFontFromFileTTF(path.c_str(), 20.0f, &font_config);

    rlImGuiReloadFonts();

    ImGuiStyle& style = ImGui::GetStyle();

    // === STRICT BLACK + CRIMSON ===
    // Deep blacks
    ImVec4 bg_0 = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
    ImVec4 bg_1 = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    ImVec4 bg_2 = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    ImVec4 bg_3 = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);

    // Crimson tones
    ImVec4 red_primary = ImVec4(0.95f, 0.20f, 0.30f, 1.00f);
    ImVec4 red_secondary = ImVec4(0.80f, 0.15f, 0.25f, 1.00f);
    ImVec4 red_highlight = ImVec4(1.00f, 0.35f, 0.45f, 1.00f);

    // Text
    ImVec4 text_primary = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    ImVec4 text_disabled = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);

    // Hover/active/select overlays
    ImVec4 hover = ImVec4(0.95f, 0.20f, 0.30f, 0.25f);
    ImVec4 active = ImVec4(0.95f, 0.20f, 0.30f, 0.70f);
    ImVec4 select = ImVec4(0.95f, 0.20f, 0.30f, 0.40f);

    // === Apply Colors ===
    style.Colors[ImGuiCol_Text] = text_primary;
    style.Colors[ImGuiCol_TextDisabled] = text_disabled;
    style.Colors[ImGuiCol_WindowBg] = bg_1;
    style.Colors[ImGuiCol_ChildBg] = bg_0;
    style.Colors[ImGuiCol_PopupBg] = bg_1;
    style.Colors[ImGuiCol_Border] = bg_3;

    // Frames & Controls
    style.Colors[ImGuiCol_FrameBg] = bg_2;
    style.Colors[ImGuiCol_FrameBgHovered] = hover;
    style.Colors[ImGuiCol_FrameBgActive] = active;
    style.Colors[ImGuiCol_CheckMark] = red_primary;
    style.Colors[ImGuiCol_SliderGrab] = red_primary;
    style.Colors[ImGuiCol_SliderGrabActive] = red_highlight;

    // Buttons
    style.Colors[ImGuiCol_Button] = bg_2;
    style.Colors[ImGuiCol_ButtonHovered] = red_secondary;
    style.Colors[ImGuiCol_ButtonActive] = red_primary;

    // Tabs
    style.Colors[ImGuiCol_Tab] = bg_2;
    style.Colors[ImGuiCol_TabHovered] = red_secondary;
    style.Colors[ImGuiCol_TabActive] = red_primary;
    style.Colors[ImGuiCol_TabUnfocused] = bg_2;
    style.Colors[ImGuiCol_TabUnfocusedActive] = red_secondary;

    // Headers
    style.Colors[ImGuiCol_Header] = bg_3;
    style.Colors[ImGuiCol_HeaderHovered] = red_secondary;
    style.Colors[ImGuiCol_HeaderActive] = red_primary;

    // Scrollbars
    style.Colors[ImGuiCol_ScrollbarBg] = bg_1;
    style.Colors[ImGuiCol_ScrollbarGrab] = bg_3;
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = red_secondary;
    style.Colors[ImGuiCol_ScrollbarGrabActive] = red_primary;

    // Highlights
    style.Colors[ImGuiCol_TextSelectedBg] = select;
    style.Colors[ImGuiCol_DragDropTarget] = red_primary;
    style.Colors[ImGuiCol_NavHighlight] = red_primary;

    // === Modern Styling ===
    style.WindowRounding = 6.0f;
    style.FrameRounding = 5.0f;
    style.ScrollbarRounding = 6.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 5.0f;

    style.WindowBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
    style.TabBorderSize = 1.0f;

    style.WindowPadding = ImVec2(10.0f, 8.0f);
    style.FramePadding = ImVec2(8.0f, 6.0f);
    style.ItemSpacing = ImVec2(8.0f, 6.0f);
    style.ItemInnerSpacing = ImVec2(5.0f, 4.0f);
    style.ScrollbarSize = 14.0f;
    style.GrabMinSize = 12.0f;

    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);

    // --- STRICT BLACK FOCUS / SELECTION FIX ---
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.0f, 0.0f, 0.0f, 0.80f); // black focus
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.0f, 0.0f, 0.0f, 0.85f); // black overlay for selected window
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.60f); // dim background in black
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.70f); // modal dim in black

    // Active states also black (instead of red/blue)
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
    style.Colors[ImGuiCol_TabActive] = red_primary;
}
