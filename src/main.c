#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
    #define HAVE_CONIO 1
#else
    #include <unistd.h>
    #include <termios.h>
    #define HAVE_TERMIOS 1
#endif

#include "todo.h"
#include "ui.h"
#include "storage.h"

// Application state
typedef enum {
    STATE_LIST_SELECTION,
    STATE_TODO_LIST,
    STATE_ADD_TODO,
    STATE_CREATE_LIST
} AppState;

static AppState appState = STATE_LIST_SELECTION;
static int selectedTodoIndex = 0;
static int listSelectionIndex = 0;
static char** currentLists = NULL;
static int currentListCount = 0;
static char currentListName[256] = "default";
static TodoList* currentTodos = NULL;

#ifdef HAVE_TERMIOS
static struct termios originalTermios;

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &originalTermios) == -1) return;
    
    struct termios raw = originalTermios;
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTermios);
}
#endif

#ifdef HAVE_CONIO
void enableRawMode() {
    // Windows console is already in raw mode when using _getch()
}

void disableRawMode() {
    // No cleanup needed for Windows
}
#endif

// Special key codes for arrow keys
#define KEY_UP    1000
#define KEY_DOWN  1001
#define KEY_LEFT  1002
#define KEY_RIGHT 1003

int getInput() {
    #ifdef HAVE_CONIO
        if (_kbhit()) {
            int ch = _getch();
            // Handle extended keys (arrow keys send two bytes: 224 + key code)
            if (ch == 224) {
                if (_kbhit()) {
                    int key = _getch();
                    // Map Windows arrow key codes
                    if (key == 72) return KEY_UP;    // Up arrow
                    if (key == 80) return KEY_DOWN;  // Down arrow
                    if (key == 77) return KEY_RIGHT; // Right arrow
                    if (key == 75) return KEY_LEFT;  // Left arrow
                }
            }
            return ch;
        }
        return -1;
    #else
        unsigned char c;
        if (read(STDIN_FILENO, &c, 1) == 1) {
            return (int)c;
        }
        return -1;
    #endif
}

void updateTodoList() {
    if (currentTodos) {
        freeTodoList(currentTodos);
    }
    currentTodos = listTodos(-1);  // -1 means show all
    
    if (selectedTodoIndex >= (currentTodos ? currentTodos->count : 0)) {
        selectedTodoIndex = (currentTodos && currentTodos->count > 0) ? currentTodos->count - 1 : 0;
    }
}

void handleListSelectionInput(int input) {
    if (input == KEY_UP) {
        listSelectionIndex = (listSelectionIndex > 0) ? listSelectionIndex - 1 : 0;
        return;
    }
    
    if (input == KEY_DOWN) {
        int maxIndex = currentListCount;
        listSelectionIndex = (listSelectionIndex < maxIndex) ? listSelectionIndex + 1 : maxIndex;
        return;
    }
    
    if (input == 27) {  // ESC key
        exit(0);
    }
    
    if (input == '\r' || input == '\n') {  // Enter
        if (listSelectionIndex == currentListCount) {
            appState = STATE_CREATE_LIST;
            disableRawMode();
            
            char listName[256];
            printf("Enter new list name: ");
            fflush(stdout);
            
            if (fgets(listName, sizeof(listName), stdin)) {
                // Remove newline
                size_t len = strlen(listName);
                if (len > 0 && listName[len-1] == '\n') {
                    listName[len-1] = '\0';
                }
                
                if (strlen(listName) > 0) {
                    if (createList(listName)) {
                        strncpy(currentListName, listName, sizeof(currentListName) - 1);
                        setCurrentList(listName);
                        updateTodoList();
                        appState = STATE_TODO_LIST;
                    } else {
                        printError("List already exists");
                        appState = STATE_LIST_SELECTION;
                    }
                } else {
                    appState = STATE_LIST_SELECTION;
                }
            }
            
            enableRawMode();
        } else if (listSelectionIndex < currentListCount) {
            strncpy(currentListName, currentLists[listSelectionIndex], sizeof(currentListName) - 1);
            setCurrentList(currentListName);
            updateTodoList();
            appState = STATE_TODO_LIST;
        }
        return;
    }
}

void handleTodoListInput(int input) {
    // Handle arrow keys
    if (input == KEY_UP) {
        selectedTodoIndex = (selectedTodoIndex > 0) ? selectedTodoIndex - 1 : 0;
        return;
    }
    
    if (input == KEY_DOWN) {
        int maxIndex = (currentTodos) ? currentTodos->count - 1 : 0;
        selectedTodoIndex = (selectedTodoIndex < maxIndex) ? selectedTodoIndex + 1 : maxIndex;
        return;
    }
    
    if (input == 27) {  // ESC key
        // Save and go back to list selection
        appState = STATE_LIST_SELECTION;
        listSelectionIndex = 0;
        
        // Reload lists
        if (currentLists) {
            freeLists(currentLists, currentListCount);
        }
        currentLists = getLists(&currentListCount);
        
        return;
    }
    
    if (input == 'i' || input == 'I') {
        appState = STATE_ADD_TODO;
        disableRawMode();
        
        char task[512];
        printf("\nEnter new task: ");
        fflush(stdout);
        
        if (fgets(task, sizeof(task), stdin)) {
            // Remove newline
            size_t len = strlen(task);
            if (len > 0 && task[len-1] == '\n') {
                task[len-1] = '\0';
            }
            
            if (strlen(task) > 0) {
                addTodo(task);
                updateTodoList();
            }
        }
        
        appState = STATE_TODO_LIST;
        enableRawMode();
        return;
    }
    
    if (input == 'd' || input == 'D') {
        if (currentTodos && selectedTodoIndex < currentTodos->count) {
            deleteTodo(selectedTodoIndex);
            updateTodoList();
        }
        return;
    }
    
    if (input == ' ') {  // Space
        if (currentTodos && selectedTodoIndex < currentTodos->count) {
            completeTodo(selectedTodoIndex);
            updateTodoList();
        }
        return;
    }
}

void render() {
    switch (appState) {
        case STATE_LIST_SELECTION:
            renderListSelection(currentLists, currentListCount, listSelectionIndex);
            break;
            
        case STATE_TODO_LIST:
            renderTodoList(currentTodos, selectedTodoIndex, currentListName);
            break;
            
        case STATE_ADD_TODO:
        case STATE_CREATE_LIST:
            // Rendering handled in input handlers
            break;
    }
}

int main() {
    initStorage();
    loadData();
    
    // Load initial lists
    currentLists = getLists(&currentListCount);
    setCurrentList(currentListName);
    updateTodoList();
    
    enableRawMode();
    
    printf("\n");
    printf("Starting TODO Application...\n");
    printf("Press any key to begin...\n");
    getInput();
    
    render();
    
    while (1) {
        int input = getInput();
        
        if (input != -1) {
            // Special case: Ctrl+C
            if (input == 3) {  // Ctrl+C
                disableRawMode();
                clearScreen();
                printf("Goodbye!\n");
                exit(0);
            }
            
            switch (appState) {
                case STATE_LIST_SELECTION:
                    handleListSelectionInput(input);
                    break;
                case STATE_TODO_LIST:
                    handleTodoListInput(input);
                    break;
                default:
                    break;
            }
            
            render();
        }
        
        #ifdef _WIN32
            Sleep(10);  // Small sleep to prevent busy waiting
        #else
            usleep(10000);
        #endif
    }
    
    disableRawMode();
    
    if (currentLists) {
        freeLists(currentLists, currentListCount);
    }
    if (currentTodos) {
        freeTodoList(currentTodos);
    }
    cleanupTodoData();
    
    return 0;
}
