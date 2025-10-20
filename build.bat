@echo off
REM Build script for Windows - CMake version
REM For GCC build, use: build-gcc.bat

echo TODO CLI - CMake Build
echo.
echo Note: This script uses CMake. If you want to use GCC directly, run:
echo   build-gcc.bat
echo.

REM Check if CMake is installed
cmake --version >nul 2>&1
if errorlevel 1 (
    echo CMake not found. Using GCC build instead...
    call build-gcc.bat
    exit /b %errorlevel%
)

echo Building TODO CLI with CMake...

REM Check if build directory exists
if not exist build mkdir build
cd build

REM Run CMake
cmake ..
if errorlevel 1 (
    echo CMake configuration failed
    cd ..
    echo.
    echo Falling back to GCC build...
    call build-gcc.bat
    exit /b %errorlevel%
)

REM Build the project
cmake --build . --config Release
if errorlevel 1 (
    echo Build failed
    cd ..
    exit /b 1
)

echo.
echo ===============================================
echo  Build successful!
echo ===============================================
echo.
echo Executable created at: build\Release\todo-cli.exe
echo.
echo To run the application:
echo   build\Release\todo-cli.exe
