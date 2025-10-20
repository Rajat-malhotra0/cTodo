cTodo - Command Line Todo Manager

A lightweight command-line todo application written in C.

Features

- Create and manage multiple todo lists
- Add, complete, and delete todos
- Navigate with arrow keys
- Persistent JSON storage
- Works on Windows, macOS, and Linux

Installation

1. Clone the repository
   git clone https://github.com/Rajat-malhotra0/cTodo.git
   cd cTodo

2. Build and setup
   Windows: build.bat
   Unix/Linux/macOS: gcc compiler with standard build tools

3. Choose setup option
   After building, the script will ask if you want to setup cTodo as a global command
   - Yes (PowerShell): Adds alias to PowerShell profile
   - Yes (Command Prompt): Adds to system PATH
   - No: Use locally from build/todo-cli.exe

Requirements

- GCC compiler
- Git

Usage

If setup globally:
   cTodo

If setup locally:
   ./build/todo-cli.exe
   or
   build\todo-cli.exe (Windows)

Keyboard Controls

UP/DOWN arrows - Navigate items
ENTER - Select
SPACE - Toggle completion
I - Add new todo
D - Delete todo
ESC - Go back/exit
Ctrl+C - Exit

Data Storage

Todos are stored in:
   Windows: C:\Users\<username>\.todo-cli-data.json
   Unix/Linux/macOS: ~/.todo-cli-data.json

Project Structure

src/ - Source code
include/ - Header files
build/ - Compiled executable
cJSON/ - JSON library (auto-downloaded)

License

MIT License
