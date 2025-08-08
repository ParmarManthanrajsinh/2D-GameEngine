cmake_minimum_required(VERSION 3.10)
project(2DGameEngine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Always build raylib from source as a SHARED library so EXE and GameLogic share the same RLGL state
include(FetchContent)
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries" FORCE)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP True
)
FetchContent_MakeAvailable(raylib)

# Collect source files only (.cpp)
file(GLOB GAME_SRC_CPP "Game/*.cpp")
file(GLOB ENGINE_SRC_CPP "Engine/*.cpp")
file(GLOB EDITOR_SRC_CPP "Editor/*.cpp")

# rlImGui and ImGui sources
set(RLIMGUI_SRC Editor/rlImGui/rlImGui.cpp)
set(IMGUI_SRC Editor/imgui/imgui.cpp Editor/imgui/imgui_draw.cpp Editor/imgui/imgui_tables.cpp Editor/imgui/imgui_widgets.cpp Editor/imgui/imgui_demo.cpp)
set(TINYFILEDIALOGS Editor/tinyfiledialogs/tinyfiledialogs.c)

# Copy assets to the build directory
if(EXISTS "${CMAKE_SOURCE_DIR}/Assets")
    file(COPY ${CMAKE_SOURCE_DIR}/Assets DESTINATION ${CMAKE_BINARY_DIR})
endif()

# Build Engine as a static library so both main and GameLogic can link to it
add_library(Engine STATIC ${ENGINE_SRC_CPP})
# Expose Engine headers and link raylib publicly so consumers inherit include dirs/libs
target_include_directories(Engine PUBLIC 
    ${CMAKE_SOURCE_DIR}/Engine
)
target_link_libraries(Engine PUBLIC raylib)

# Create GameLogic shared library (DLL)
file(GLOB GAMELOGIC_SRC_CPP "GameLogic/*.cpp")
add_library(GameLogic SHARED ${GAMELOGIC_SRC_CPP})
# GameLogic needs Engine headers and linkage (for GameMap symbols)
target_include_directories(GameLogic PRIVATE 
    ${CMAKE_SOURCE_DIR}/Engine
    ${CMAKE_SOURCE_DIR}/GameLogic
)
target_link_libraries(GameLogic PRIVATE Engine)

# Create main executable (Game Editor) without Engine sources; link against Engine
add_executable(main 
    ${GAME_SRC_CPP} 
    ${EDITOR_SRC_CPP} 
    ${RLIMGUI_SRC} 
    ${IMGUI_SRC} 
    ${TINYFILEDIALOGS}
)

# Add include directories for the main executable
target_include_directories(main PRIVATE 
    ${CMAKE_SOURCE_DIR}/Editor/imgui 
    ${CMAKE_SOURCE_DIR}/Editor/rlImGui 
    ${CMAKE_SOURCE_DIR}/Editor/tinyfiledialogs
    ${CMAKE_SOURCE_DIR}/Engine
    ${CMAKE_SOURCE_DIR}/Editor
    ${CMAKE_SOURCE_DIR}/Game
)

# Link libraries
# Link Engine (which brings raylib transitively)
target_link_libraries(main PRIVATE Engine)

# Copy GameLogic DLL beside main executable after building 'main'.
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:GameLogic>
            $<TARGET_FILE_DIR:main>)

# Also copy raylib DLL so both EXE and GameLogic load the same shared raylib
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:raylib>
            $<TARGET_FILE_DIR:main>)

# When building GameLogic alone, also copy it next to the main executable
add_custom_command(TARGET GameLogic POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:GameLogic>
            ${CMAKE_BINARY_DIR})
