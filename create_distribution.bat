@echo off
REM Distribution Creation Script for 2D Game Engine
REM This script must be run from a Visual Studio Developer Command Prompt

echo Creating 2D Game Engine Distribution Package...
echo.

REM Check if we're in a VS Developer environment
where cl.exe >nul 2>&1
if errorlevel 1 (
    echo ERROR: This script must be run from a Visual Studio Developer Command Prompt
    echo.
    echo To fix this:
    echo 1. Open "Developer Command Prompt for VS" from Start Menu
    echo 2. Navigate to this project directory
    echo 3. Run this script again
    echo.
    pause
    exit /b 1
)

echo Visual Studio environment detected...

REM Build release version if it doesn't exist
if not exist "out\build\x64-release\main.exe" (
    echo Building release version...
    cmake --preset x64-release
    if errorlevel 1 (
        echo ERROR: CMake configuration failed!
        pause
        exit /b 1
    )
    
    cmake --build out\build\x64-release --config Release --target main
    if errorlevel 1 (
        echo ERROR: Build failed!
        pause
        exit /b 1
    )
)

echo Creating distribution using PowerShell script...
powershell -ExecutionPolicy Bypass -File ".\distribute.ps1"

echo.
echo Distribution created successfully!
echo You can now copy the 'dist' folder to distribute your game engine.
echo.
pause