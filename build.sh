#!/bin/bash

# Build script for Unix/Linux/macOS
# Supports both CMake and GCC builds

echo "TODO CLI Build Script"
echo ""

# Check for GCC option
if [ "$1" = "gcc" ] || [ "$1" = "gcc-only" ]; then
    echo "Using GCC build..."
    echo ""
    
    # Check if cJSON needs to be cloned
    if [ ! -d "cJSON" ]; then
        echo "Cloning cJSON library..."
        git clone https://github.com/DaveGamble/cJSON.git
        if [ $? -ne 0 ]; then
            echo "Failed to clone cJSON"
            exit 1
        fi
    fi
    
    # Compile cJSON if needed
    if [ ! -f "cJSON/cJSON.o" ]; then
        echo "Compiling cJSON..."
        cd cJSON
        gcc -c -std=c99 cJSON.c -o cJSON.o
        if [ $? -ne 0 ]; then
            echo "Failed to compile cJSON"
            exit 1
        fi
        cd ..
    fi
    
    # Compile the application
    echo "Compiling TODO CLI with GCC..."
    mkdir -p build
    gcc -std=c99 -Wall -Wextra -I./include -I./cJSON \
        -o build/todo-cli \
        src/main.c src/todo.c src/storage.c src/ui.c \
        cJSON/cJSON.o
    
    if [ $? -ne 0 ]; then
        echo "Build failed"
        exit 1
    fi
    
    echo ""
    echo "Build successful!"
    echo "Executable created at: build/todo-cli"
    echo ""
    echo "To run the application:"
    echo "  ./build/todo-cli"
    exit 0
fi

# CMake build (default)
echo "Building TODO CLI with CMake..."
echo ""

# Check if CMake is available
if ! command -v cmake &> /dev/null; then
    echo "CMake not found. Building with GCC instead..."
    echo ""
    exec "$0" gcc
fi

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

# Run CMake
cmake ..
if [ $? -ne 0 ]; then
    echo "CMake configuration failed"
    cd ..
    echo ""
    echo "Trying GCC build instead..."
    exec "$0" gcc
fi

# Build the project
cmake --build . --config Release
if [ $? -ne 0 ]; then
    echo "Build failed"
    exit 1
fi

echo ""
echo "Build successful!"
echo "Executable created at: build/todo-cli"
echo ""
echo "To run the application:"
echo "  ./build/todo-cli"
echo ""
echo "For GCC-only build, run: ./build.sh gcc
