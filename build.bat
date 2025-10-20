@echo off
setlocal enabledelayedexpansion

cls
echo.
echo ==================================================
echo            cTodo - Build and Setup
echo ==================================================
echo.

REM Check if cJSON exists
if not exist cJSON (
    echo Downloading cJSON library...
    git clone https://github.com/DaveGamble/cJSON.git
    if errorlevel 1 (
        echo Error: Failed to clone cJSON
        echo Make sure git is installed
        pause
        exit /b 1
    )
)

REM Compile cJSON if not already compiled
if not exist cJSON\cJSON.o (
    echo Compiling cJSON...
    cd cJSON
    gcc -c -std=c99 cJSON.c -o cJSON.o
    if errorlevel 1 (
        echo Error: Failed to compile cJSON
        echo Make sure GCC is installed
        cd ..
        pause
        exit /b 1
    )
    cd ..
)

REM Create build directory
if not exist build mkdir build

REM Compile the application
echo Compiling cTodo...
gcc -std=c99 -Wall -Wextra -I.\include -I.\cJSON ^
    -o build\todo-cli.exe ^
    src\main.c src\todo.c src\storage.c src\ui.c ^
    cJSON\cJSON.o

if errorlevel 1 (
    echo.
    echo Error: Compilation failed
    pause
    exit /b 1
)

echo.
echo ==================================================
echo            Build Successful
echo ==================================================
echo.
echo Executable created at: build\todo-cli.exe
echo.

:setup_prompt
echo Would you like to setup 'cTodo' as a global command?
echo.
echo 1 - Yes, setup globally (PowerShell)
echo 2 - Yes, setup globally (Command Prompt)
echo 3 - No, just use locally
echo.
set /p choice="Enter choice (1-3): "

if "%choice%"=="1" (
    cls
    echo Setting up cTodo for PowerShell...
    echo.
    
    for %%I in ("%cd%\build\todo-cli.exe") do set exePath=%%~fI
    
    REM Create PowerShell profile directory if it doesn't exist
    powershell -NoProfile -Command "$profileDir = Split-Path -Parent $PROFILE.CurrentUserCurrentHost; if (-not (Test-Path $profileDir)) { New-Item -ItemType Directory -Path $profileDir -Force | Out-Null }"
    
    REM Add alias to PowerShell profile
    powershell -NoProfile -Command "Add-Content -Path $PROFILE.CurrentUserCurrentHost -Value ([Environment]::NewLine + '# cTodo command' + [Environment]::NewLine + 'Set-Alias -Name cTodo -Value \'%exePath%\' -Scope Global -Force') -Encoding UTF8; Write-Host 'Setup complete! Restart PowerShell and type: cTodo'"
    
    pause
    goto end
)

if "%choice%"=="2" (
    cls
    echo Setting up cTodo for Command Prompt...
    echo.
    
    REM Get the build directory path
    for %%I in ("%cd%\build") do set buildDir=%%~fI
    
    echo Adding !buildDir! to PATH...
    setx PATH "!PATH!;!buildDir!"
    
    if errorlevel 1 (
        echo.
        echo Error: Failed to add to PATH
        echo Try running this script as administrator
        pause
        exit /b 1
    )
    
    echo.
    echo Setup complete! Restart Command Prompt and type: cTodo
    pause
    goto end
)

if "%choice%"=="3" (
    cls
    echo.
    echo Local setup complete!
    echo.
    echo To run cTodo, use:
    echo   %cd%\build\todo-cli.exe
    echo.
    echo Or from this directory:
    echo   build\todo-cli.exe
    echo.
    pause
    goto end
)

echo Invalid choice
goto setup_prompt

:end
cls
echo Build and setup complete!
echo.
echo For more information, see README.md
echo.
