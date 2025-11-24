# Simple Game Export Script with Asset Support
# Creates only the essential game runtime files: app.exe, GameLogic.dll, raylib.dll + Assets

param(
    [string]$BuildConfig = "Release",
    [string]$OutputDir = "export"
)

$ErrorActionPreference = "Stop"

Write-Host "Creating simple game export with assets..." -ForegroundColor Green
Write-Host "Build Config: $BuildConfig, Output Directory: $OutputDir" -ForegroundColor Cyan

# Build path
$BuildPath = "out/build/x64-$($BuildConfig.ToLower())"

# Build the game runtime only (not the editor)
Write-Host "Building game runtime..." -ForegroundColor Yellow
cmake --build $BuildPath --config $BuildConfig --target game GameLogic
if ($LASTEXITCODE -ne 0) {
    throw "Build failed with exit code $LASTEXITCODE"
}

# Verify the files exist
$GameExe = Join-Path $BuildPath "game.exe"
$LogicDll = Join-Path $BuildPath "GameLogic.dll"
$RaylibDll = Join-Path $BuildPath "raylib.dll"

if (-not (Test-Path $GameExe)) { throw "Missing game.exe at $GameExe" }
if (-not (Test-Path $LogicDll)) { throw "Missing GameLogic.dll at $LogicDll" }
if (-not (Test-Path $RaylibDll)) { throw "Missing raylib.dll at $RaylibDll" }

# Create clean export directory
if (Test-Path $OutputDir) {
    Write-Host "Cleaning export directory..." -ForegroundColor Yellow
    Remove-Item -Path $OutputDir -Recurse -Force
}
New-Item -ItemType Directory -Path $OutputDir -Force | Out-Null

# Copy only the essential runtime files
Write-Host "Copying game runtime files..." -ForegroundColor Yellow
Copy-Item $GameExe "$OutputDir/app.exe" -Force
Copy-Item $LogicDll "$OutputDir/" -Force  
Copy-Item $RaylibDll "$OutputDir/" -Force

# Copy Assets folder if it exists (excluding editor icons)
if (Test-Path "Assets") {
    Write-Host "Copying game assets..." -ForegroundColor Yellow
    New-Item -ItemType Directory -Path "$OutputDir/Assets" -Force | Out-Null
    
    # Copy all items from Assets except the icons folder
    Get-ChildItem "Assets" | Where-Object { $_.Name -ne "icons" } | ForEach-Object {
        Copy-Item $_.FullName "$OutputDir/Assets" -Recurse -Force
    }
    Write-Host "Game assets copied successfully (excluding editor icons)" -ForegroundColor Green
} else {
    Write-Host "No Assets folder found - skipping asset copy" -ForegroundColor Yellow
}

Write-Host "Simple game export completed successfully!" -ForegroundColor Green
Write-Host ""
Write-Host "Export contains:" -ForegroundColor Cyan
Write-Host "- app.exe (game runtime)" -ForegroundColor White
Write-Host "- GameLogic.dll (hot-reloadable game logic)" -ForegroundColor White  
Write-Host "- raylib.dll (graphics library)" -ForegroundColor White
if (Test-Path "$OutputDir/Assets") {
    $AssetCount = (Get-ChildItem "$OutputDir/Assets" -Recurse -File).Count
    Write-Host "- Assets/ folder ($AssetCount files)" -ForegroundColor White
}