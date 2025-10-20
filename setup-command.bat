@echo off
REM Setup script to add cTodo command to Windows PATH

echo.
echo ================================================
echo  cTodo CLI Setup - Add command to PATH
echo ================================================
echo.

REM Get the directory where this script is located
for %%I in ("%~dp0.") do set "projectDir=%%~fI"
set "todoExePath=%projectDir%\build\todo-cli.exe"

REM Check if the executable exists
if not exist "%todoExePath%" (
    echo Error: todo-cli.exe not found at:
    echo   %todoExePath%
    echo.
    echo Please build the project first:
    echo   cd c-impl
    echo   build-gcc.bat
    echo.
    pause
    exit /b 1
)

echo Found executable at:
echo   %todoExePath%
echo.

REM Get the build directory (where todo-cli.exe is)
for %%I in ("%todoExePath%\..") do set "buildDir=%%~fI"

echo This script will add the following to your PATH:
echo   %buildDir%
echo.

echo You have two options:
echo.
echo 1. User-level (current user only, recommended):
echo    - Click YES to proceed
echo.
echo 2. System-level (all users):
echo    - Cancel, then right-click and select "Run as administrator"
echo    - Run this script again
echo.

REM Try to add to user PATH
echo Adding to user PATH...
setx PATH "%PATH%;%buildDir%"

if errorlevel 1 (
    echo.
    echo Error: Failed to add to PATH
    echo.
    echo Try running as administrator:
    echo   Right-click cmd.exe or PowerShell and select "Run as administrator"
    echo   Then run this script again
    pause
    exit /b 1
)

echo.
echo ================================================
echo  Setup Complete!
echo ================================================
echo.
echo The 'cTodo' command has been added to your PATH.
echo.
echo IMPORTANT: You must restart your terminal for the changes to take effect!
echo.
echo Usage:
echo   cTodo          - Opens the todo application
echo.
echo After restarting, you can use 'cTodo' from any directory.
echo.

echo Would you like to test the command now? (y/n)
set /p testCmd=

if /i "%testCmd%"=="y" (
    echo.
    echo Starting cTodo...
    echo.
    "%todoExePath%"
)

pause
