# Distribution Script for 2D Game Engine
# This script creates a distribution package with the engine executable and development environment

param(
    [string]$BuildConfig = "Release",
    [string]$OutputDir = "dist"
)

Write-Host "Creating distribution package..." -ForegroundColor Green

# Ensure we have a release build
$BuildPath = "out/build/x64-$($BuildConfig.ToLower())"
if (-not (Test-Path $BuildPath)) {
    Write-Host "Building release version..." -ForegroundColor Yellow
    cmake --preset x64-release
    cmake --build out/build/x64-release --config Release --target main
}

# Create distribution directory structure
$DistPath = $OutputDir
if (Test-Path $DistPath) {
    Remove-Item $DistPath -Recurse -Force
}
New-Item -ItemType Directory -Path $DistPath -Force | Out-Null
New-Item -ItemType Directory -Path "$DistPath/GameLogic" -Force | Out-Null
New-Item -ItemType Directory -Path "$DistPath/Assets" -Force | Out-Null
New-Item -ItemType Directory -Path "$DistPath/Engine" -Force | Out-Null

Write-Host "Copying executable and dependencies..." -ForegroundColor Yellow

# Copy main executable
Copy-Item "$BuildPath/main.exe" "$DistPath/app.exe"

# Copy required DLLs
Copy-Item "$BuildPath/raylib.dll" "$DistPath/"
Copy-Item "$BuildPath/GameLogic.dll" "$DistPath/"

# Copy Assets folder
Copy-Item "Assets/*" "$DistPath/Assets/" -Recurse

Write-Host "Creating development environment..." -ForegroundColor Yellow

# Copy GameLogic source files
Copy-Item "GameLogic/*" "$DistPath/GameLogic/" -Recurse

# Copy Engine headers and source files (needed for GameLogic development)
Copy-Item "Engine/*.h" "$DistPath/Engine/"
Copy-Item "Engine/*.cpp" "$DistPath/Engine/"

# Copy the distribution CMakeLists.txt
Copy-Item "dist_CMakeLists.txt" "$DistPath/CMakeLists.txt"

# Copy distribution documentation
Copy-Item "README_DISTRIBUTION.md" "$DistPath/"

# Copy build helper script
Copy-Item "build_gamelogic.bat" "$DistPath/"

Write-Host "Creating build configuration..." -ForegroundColor Yellow

Write-Host "Distribution created successfully in '$DistPath'" -ForegroundColor Green
Write-Host ""
Write-Host "Distribution contents:" -ForegroundColor Cyan
Write-Host "- app.exe (main game engine/editor)" -ForegroundColor White
Write-Host "- raylib.dll (graphics library)" -ForegroundColor White
Write-Host "- GameLogic.dll (hot-reloadable game logic)" -ForegroundColor White
Write-Host "- CMakeLists.txt (for building GameLogic)" -ForegroundColor White
Write-Host "- build_gamelogic.bat (quick build helper)" -ForegroundColor White
Write-Host "- GameLogic/ (source code for game development)" -ForegroundColor White
Write-Host "- Engine/ (engine headers)" -ForegroundColor White
Write-Host "- Assets/ (game assets)" -ForegroundColor White
Write-Host "- README_DISTRIBUTION.md (usage instructions)" -ForegroundColor White