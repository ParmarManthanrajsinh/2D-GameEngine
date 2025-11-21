@echo off
REM Quick build script for GameLogic.dll
REM This script helps users rebuild their game logic for hot-reloading

echo Building GameLogic.dll...

REM Check if build directory exists
if not exist build (
    echo Creating build directory...
    mkdir build
)

cd build

REM Configure CMake if not already done
if not exist CMakeCache.txt (
    echo Configuring CMake...
    cmake ..
    if errorlevel 1 (
        echo ERROR: CMake configuration failed!
        echo Make sure you're running this from a Developer Command Prompt
        pause
        exit /b 1
    )
)

REM Build GameLogic
echo Building GameLogic target...
cmake --build . --target GameLogic --config Release
if errorlevel 1 (
    echo ERROR: Build failed!
    pause
    exit /b 1
)

cd ..
echo.
echo GameLogic.dll built successfully!
echo The engine will automatically detect and reload the changes.
echo.
pause