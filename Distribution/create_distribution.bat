@echo off
echo Creating 2D Game Engine Distribution Package...
echo.

REM Check if VS tools are available, if not try to auto-setup
where cl.exe >nul 2>&1
if errorlevel 1 goto SETUP_VS

REM Check if we have the right architecture
if defined VSCMD_ARG_TGT_ARCH (
    if /I "%VSCMD_ARG_TGT_ARCH%"=="x64" goto ARCH_OK
    echo Wrong architecture detected: %VSCMD_ARG_TGT_ARCH%. Setting up x64 environment...
    goto SETUP_VS
)

echo Visual Studio tools detected but architecture unknown. Setting up x64 environment...

:SETUP_VS
echo Setting up Visual Studio x64 environment...

REM Try common VS 2022 locations
set "VCVARS64="
if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat" (
    set "VCVARS64=C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" (
    set "VCVARS64=C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
    set "VCVARS64=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
) else (
    echo ERROR: Could not find Visual Studio 2022 installation.
    echo Please install Visual Studio 2022 with C++ tools or run from "x64 Native Tools Command Prompt".
    echo Checked paths:
    echo - C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat
    echo - C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat  
    echo - C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat
    if "%~2" NEQ "nopause" pause
    exit /b 1
)

echo Found VS tools at: %VCVARS64%
echo Calling vcvars64.bat to set up environment...
call "%VCVARS64%"
if errorlevel 1 (
    echo ERROR: Failed to set up Visual Studio environment.
    if "%~2" NEQ "nopause" pause
    exit /b 1
)
echo VS environment setup completed.

:ARCH_OK
echo Visual Studio x64 environment ready.

REM Build release version if missing
if not exist "out\build\x64-release\main.exe" (
    echo Building release version...
    cmake --preset x64-release
    if errorlevel 1 (
        echo ERROR: CMake configuration failed!
        if "%~2" NEQ "nopause" pause
        exit /b 1
    )
    
    cmake --build out\build\x64-release --config Release --target main
    if errorlevel 1 (
        echo ERROR: Build failed!
        if "%~2" NEQ "nopause" pause
        exit /b 1
    )
)

echo Creating distribution using PowerShell script...
echo Parameters: OutputDir="%~1" BuildConfig="%~3" NoPause="%~2"
if "%~1"=="" (
    if "%~3"=="" (
        echo Running: powershell.exe -ExecutionPolicy Bypass -File ".\Distribution\distribute.ps1"
        powershell.exe -ExecutionPolicy Bypass -File ".\Distribution\distribute.ps1"
    ) else (
        echo Running: powershell.exe -ExecutionPolicy Bypass -File ".\Distribution\distribute.ps1" -BuildConfig "%~3"
        powershell.exe -ExecutionPolicy Bypass -File ".\Distribution\distribute.ps1" -BuildConfig "%~3"
    )
) else (
    if "%~3"=="" (
        echo Running: powershell.exe -ExecutionPolicy Bypass -File ".\Distribution\distribute.ps1" -OutputDir "%~1"
        powershell.exe -ExecutionPolicy Bypass -File ".\Distribution\distribute.ps1" -OutputDir "%~1"
    ) else (
        echo Running: powershell.exe -ExecutionPolicy Bypass -File ".\Distribution\distribute.ps1" -OutputDir "%~1" -BuildConfig "%~3"
        powershell.exe -ExecutionPolicy Bypass -File ".\Distribution\distribute.ps1" -OutputDir "%~1" -BuildConfig "%~3"
    )
)
echo PowerShell script finished with exit code: %ERRORLEVEL%
if errorlevel 1 (
    echo ERROR: Distribution failed.
    if "%~2" NEQ "nopause" pause
    exit /b 1
)

echo Distribution created successfully!
if "%~2" NEQ "nopause" pause
