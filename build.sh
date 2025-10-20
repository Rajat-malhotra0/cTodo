#!/bin/bash

# Linux Build Script for cTodo
# Compiles the C application and sets up global 'ctodo' command

set -e

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}==================================================
            cTodo - Build for Linux
==================================================${NC}\n"

# Check if cJSON exists
if [ ! -d "cJSON" ]; then
    echo "Downloading cJSON library..."
    git clone https://github.com/DaveGamble/cJSON.git
fi

# Compile cJSON if not already compiled
if [ ! -f "cJSON/cJSON.o" ]; then
    echo "Compiling cJSON..."
    cd cJSON
    gcc -c -std=c99 cJSON.c -o cJSON.o
    cd ..
fi

# Create build directory
mkdir -p build

# Compile the application
echo "Compiling cTodo..."
BUILD_DIR="$(cd build && pwd)"
EXE_PATH="$BUILD_DIR/todo-cli"

gcc -std=c99 -Wall -Wextra -I./include -I./cJSON \
    -o "$EXE_PATH" \
    src/main.c src/todo.c src/storage.c src/ui.c \
    cJSON/cJSON.o

echo -e "\n${GREEN}==================================================
            Build Successful
==================================================${NC}"
echo "Executable created at: $EXE_PATH"
echo ""

# Setup global command
echo -e "${YELLOW}Setting up 'ctodo' as a global command...${NC}\n"

# Create symlink in /usr/local/bin or use bin directory approach
if [ -w "/usr/local/bin" ]; then
    sudo ln -sf "$EXE_PATH" /usr/local/bin/ctodo
    echo -e "${GREEN}Setup complete!${NC}"
    echo "You can now type: ${YELLOW}ctodo${NC} from anywhere"
else
    # Alternative: add to home bin directory
    mkdir -p "$HOME/.local/bin"
    ln -sf "$EXE_PATH" "$HOME/.local/bin/ctodo"
    
    # Check if ~/.local/bin is in PATH
    if [[ ":$PATH:" != *":$HOME/.local/bin:"* ]]; then
        echo -e "${YELLOW}Note: $HOME/.local/bin is not in your PATH${NC}"
        echo "Add this line to your shell profile (~/.bashrc, ~/.zshrc, etc.):"
        echo "export PATH=\"\$HOME/.local/bin:\$PATH\""
        echo ""
        echo "Then run: source ~/.bashrc (or your shell profile)"
    fi
    
    echo -e "${GREEN}Setup complete!${NC}"
    echo "You can now type: ${YELLOW}ctodo${NC} from anywhere"
fi
