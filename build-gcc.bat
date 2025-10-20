@echo off
REM GCC Build script for Windows
REM Updated for new directory structure (C code in root)

echo TODO CLI - GCC Build
echo.

REM Check if cJSON exists
if not exist cJSON (
    echo Cloning cJSON library...
    git clone https://github.com/DaveGamble/cJSON.git
    if errorlevel 1 (
        echo Failed to clone cJSON - make sure git is installed
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
        echo Failed to compile cJSON - make sure gcc is installed
        cd ..
        pause
        exit /b 1
    )
    cd ..
)

REM Create build directory
if not exist build mkdir build

REM Compile the application
echo Compiling TODO CLI...
gcc -std=c99 -Wall -Wextra -I.\include -I.\cJSON ^
    -o build\todo-cli.exe ^
    src\main.c src\todo.c src\storage.c src\ui.c ^
    cJSON\cJSON.o

if errorlevel 1 (
    echo.
    echo Build failed! Check the errors above.
    echo.
    echo Make sure you have:
    echo - GCC installed (MinGW or similar)
    echo - Git installed
    echo - All source files present
    pause
    exit /b 1
)

echo.
echo ===============================================
echo  Build successful!
echo ===============================================
echo.
echo Executable created at: build\todo-cli.exe
echo.
echo To run the application:
echo   .\build\todo-cli.exe
echo.
echo Or double-click build\todo-cli.exe
echo.

REM Ask if user wants to run it
set /p runapp="Run the application now? (y/n): "
if /i "%runapp%"=="y" (
    echo.
    echo Starting TODO CLI...
    echo.
    .\build\todo-cli.exe
)

echo.
echo Done!
pause