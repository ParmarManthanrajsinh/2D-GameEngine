cmake_minimum_required(VERSION 3.10)
project(2DGameEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Try to find a locally installed raylib
find_package(raylib 5.5 QUIET)

include(FetchContent)
if (NOT raylib_FOUND)
    FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP True
    )
    FetchContent_MakeAvailable(raylib)
endif()

# Collect source files only (.cpp)
file(GLOB PROJECT_SRC_CPP "Game/*.cpp" "Engine/*.cpp" "Editor/*.cpp")

# rlImGui and ImGui sources
set(RLIMGUI_SRC Editor/rlImGui/rlImGui.cpp)
set(IMGUI_SRC Editor/imgui/imgui.cpp Editor/imgui/imgui_draw.cpp Editor/imgui/imgui_tables.cpp Editor/imgui/imgui_widgets.cpp Editor/imgui/imgui_demo.cpp)
set(TINYFILEDIALOGS Editor/tinyfiledialogs/tinyfiledialogs.c)

# Copy assets to the build directory
if(EXISTS "${CMAKE_SOURCE_DIR}/Assets")
    file(COPY ${CMAKE_SOURCE_DIR}/Assets DESTINATION ${CMAKE_BINARY_DIR})
endif()

# Create executable
add_executable(main ${PROJECT_SRC_CPP} ${RLIMGUI_SRC} ${IMGUI_SRC} ${TINYFILEDIALOGS})

# Add include directories — this is what lets you include .hpp/.h files
target_include_directories(main PRIVATE 
    ${raylib_SOURCE_DIR}/Game 
    ${CMAKE_SOURCE_DIR}/Editor/imgui 
    ${CMAKE_SOURCE_DIR}/Editor/rlImGui 
    ${CMAKE_SOURCE_DIR}/Editor/tinyfiledialogs
    ${CMAKE_SOURCE_DIR}/Engine
    ${CMAKE_SOURCE_DIR}/Editor
    ${CMAKE_SOURCE_DIR}/Game
)

# Link libraries
target_link_libraries(main raylib)
