cTodo - Command Line Todo Manager

A fast, lightweight todo application written in C. Manage your todos from your terminal.

Features

- Create and manage multiple todo lists
- Add, complete, and delete todos
- Navigate with arrow keys
- Persistent JSON storage
- Works on Windows, macOS, and Linux

Quick Start

1. Build the application
   cd to the project directory
   Run: build-gcc.bat (Windows) or ./build.sh (Unix/Linux/macOS)

2. Setup the command
   PowerShell: .\setup-command.ps1
   Command Prompt: setup-command.bat

3. Use it
   Restart your terminal
   Type: cTodo

Keyboard Controls

UP/DOWN arrows - Navigate items
ENTER - Select list or item
SPACE - Mark todo done/undone
I - Add new todo
D - Delete todo
ESC - Go back/exit
Ctrl+C - Exit application

Data Storage

Todos are stored at:
  Windows: C:\Users\<username>\.todo-cli-data.json
  Unix/Linux/macOS: ~/.todo-cli-data.json

Installation

For detailed installation instructions, see INSTALL.md

Requirements

- GCC compiler
- Git
- Windows 10+, macOS, or Linux

Building

Method 1: GCC (Recommended)
  ./build-gcc.bat (Windows)
  ./build.sh (Unix/Linux/macOS)

Method 2: CMake
  mkdir build && cd build
  cmake .. && cmake --build .

Usage

Type 'cTodo' from anywhere after setup to launch the application.

Project Structure

src/ - C source files
include/ - Header files
build/ - Compiled executable
cJSON/ - JSON library

License

MIT License

For more information, see the documentation files:
- INSTALL.md - Installation guide
- QUICK_START.md - Quick setup guide
- BUILD_ENV_CHECK.md - Environment verification
