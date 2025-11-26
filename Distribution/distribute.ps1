# Distribution Script for 2D Game Engine
# This script creates a distribution package with the engine executable and development environment

param(
    [string]$BuildConfig = "Release",
    [string]$OutputDir = "dist"
)

$ErrorActionPreference = "Stop"

Write-Host "Creating distribution package..." -ForegroundColor Green
Write-Host "Build Config: $BuildConfig, Output Directory: $OutputDir" -ForegroundColor Cyan

# Note: VS environment setup is handled by the calling batch script

# Ensure we have a release build
$BuildPath = "out/build/x64-$($BuildConfig.ToLower())"
if (-not (Test-Path $BuildPath)) {
    Write-Host "Configuring $BuildConfig version..." -ForegroundColor Yellow
    if ($BuildConfig -ieq "Release") {
        cmake --preset x64-release
    } else {
        cmake --preset x64-debug
    }
}

Write-Host "Building targets (game runtime, GameLogic, and editor)..." -ForegroundColor Yellow
cmake --build $BuildPath --config $BuildConfig --target game GameLogic main
if ($LASTEXITCODE -ne 0) {
    throw "Build failed with exit code $LASTEXITCODE"
}

# Verify expected outputs exist before packaging
$GameExe = Join-Path $BuildPath "game.exe"
$EditorExe = Join-Path $BuildPath "main.exe"
$LogicDll = Join-Path $BuildPath "GameLogic.dll"
$RaylibDll = Join-Path $BuildPath "raylib.dll"

if (-not (Test-Path $GameExe)) { throw "Missing game.exe at $GameExe" }
if (-not (Test-Path $LogicDll)) { throw "Missing GameLogic.dll at $LogicDll" }
if (-not (Test-Path $RaylibDll)) { throw "Missing raylib.dll at $RaylibDll" }

# Create distribution directory structure
$DistPath = $OutputDir
if (Test-Path $DistPath) {
    Write-Host "Cleaning old distribution..." -ForegroundColor Yellow
    # Remove everything except the build folder (which might be locked)
    Get-ChildItem -Path $DistPath -Exclude "build" | Remove-Item -Recurse -Force -ErrorAction SilentlyContinue
}

New-Item -ItemType Directory -Path $DistPath -Force | Out-Null
New-Item -ItemType Directory -Path "$DistPath/GameLogic" -Force | Out-Null
New-Item -ItemType Directory -Path "$DistPath/Assets" -Force | Out-Null
New-Item -ItemType Directory -Path "$DistPath/Engine" -Force | Out-Null
New-Item -ItemType Directory -Path "$DistPath/Documentation" -Force | Out-Null

# Create raylib directory structure
New-Item -ItemType Directory -Path "$DistPath/raylib/include" -Force | Out-Null
New-Item -ItemType Directory -Path "$DistPath/raylib/lib" -Force | Out-Null
New-Item -ItemType Directory -Path "$DistPath/raylib/bin" -Force | Out-Null

Write-Host "Copying executable and dependencies..." -ForegroundColor Yellow

# Copy game runtime as game.exe
Copy-Item "$BuildPath/game.exe" "$DistPath/game.exe" -Force

# Optionally include the editor (rename to editor.exe)
if (Test-Path "$BuildPath/main.exe") {
    Copy-Item "$BuildPath/main.exe" "$DistPath/editor.exe" -Force
}

# Copy GameLogic DLL to root
Copy-Item "$BuildPath/GameLogic.dll" "$DistPath/" -Force

# Copy raylib files to raylib folder structure
Copy-Item "$BuildPath/raylib.dll" "$DistPath/raylib/bin/" -Force
Copy-Item "$BuildPath/_deps/raylib-build/raylib/raylib.lib" "$DistPath/raylib/lib/" -Force
Copy-Item "$BuildPath/_deps/raylib-build/raylib/include/*.h" "$DistPath/raylib/include/" -Force

# IMPORTANT: Copy raylib.dll to dist root so app.exe can find it at runtime
Copy-Item "$BuildPath/raylib.dll" "$DistPath/" -Force

# Copy Assets folder
Copy-Item "Assets/*" "$DistPath/Assets/" -Recurse -Force

Write-Host "Creating development environment..." -ForegroundColor Yellow

# Copy GameLogic source files
Copy-Item "GameLogic/*" "$DistPath/GameLogic/" -Recurse -Force

# Copy Engine headers and source files (needed for GameLogic development)
Copy-Item "Engine/*.h" "$DistPath/Engine/" -Force
Copy-Item "Engine/*.cpp" "$DistPath/Engine/" -Force

# Copy the distribution CMakeLists.txt
Copy-Item "Distribution/dist_CMakeLists.txt" "$DistPath/CMakeLists.txt" -Force

# Copy distribution documentation
Copy-Item "Documentation/README_DISTRIBUTION.md" "$DistPath/Documentation/" -Force
Copy-Item "Documentation/DISTRIBUTION_GUIDE.md" "$DistPath/Documentation/" -Force

# Copy build helper script
Copy-Item "Distribution/build_gamelogic.bat" "$DistPath/" -Force

# Copy default game configuration
Copy-Item "Distribution/config.ini" "$DistPath/" -Force

Write-Host "Creating build configuration..." -ForegroundColor Yellow

Write-Host "Distribution created successfully in '$DistPath'" -ForegroundColor Green
Write-Host ""
Write-Host "Distribution contents:" -ForegroundColor Cyan
Write-Host "- game.exe (main game engine/editor)" -ForegroundColor White
Write-Host "- GameLogic.dll (hot-reloadable game logic)" -ForegroundColor White
Write-Host "- raylib.dll (required at runtime)" -ForegroundColor White
Write-Host "- config.ini (window and game settings)" -ForegroundColor White
Write-Host "- raylib/ (raylib development files)" -ForegroundColor White
Write-Host "  - include/ (raylib headers)" -ForegroundColor White
Write-Host "  - lib/ (raylib.lib)" -ForegroundColor White
Write-Host "  - bin/ (raylib.dll)" -ForegroundColor White
Write-Host "- CMakeLists.txt (for building GameLogic)" -ForegroundColor White
Write-Host "- build_gamelogic.bat (quick build helper)" -ForegroundColor White
Write-Host "- GameLogic/ (source code for game development)" -ForegroundColor White
Write-Host "- Engine/ (engine headers)" -ForegroundColor White
Write-Host "- Assets/ (game assets)" -ForegroundColor White
Write-Host "- Documentation/ (user guides and documentation)" -ForegroundColor White