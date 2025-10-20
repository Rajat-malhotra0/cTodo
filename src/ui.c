#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ANSI color codes
#define ANSI_RESET      "\033[0m"
#define ANSI_BOLD       "\033[1m"
#define ANSI_DIM        "\033[2m"
#define ANSI_STRIKE     "\033[9m"

#define ANSI_BLUE       "\033[94m"
#define ANSI_GREEN      "\033[92m"
#define ANSI_RED        "\033[91m"
#define ANSI_CYAN       "\033[96m"
#define ANSI_WHITE      "\033[97m"
#define ANSI_GRAY       "\033[90m"

#define ANSI_BG_BLUE    "\033[104m"

void clearScreen() {
    // Use cls command for Windows
    system("cls");
}

void printHeader(const char* text) {
    if (!text) return;
    
    int len = strlen(text);
    printf("%s+", ANSI_BLUE);
    for (int i = 0; i < len + 2; i++) printf("-");
    printf("+%s\n", ANSI_RESET);
    
    printf("%s|%s %s %s|%s\n", ANSI_BLUE, ANSI_BOLD, text, ANSI_BLUE, ANSI_RESET);
    
    printf("%s+", ANSI_BLUE);
    for (int i = 0; i < len + 2; i++) printf("-");
    printf("+%s\n", ANSI_RESET);
}

void printSuccess(const char* message) {
    printf("%s✓ %s%s%s\n", ANSI_GREEN, ANSI_WHITE, message, ANSI_RESET);
}

void printError(const char* message) {
    printf("%s✗ %s%s%s\n", ANSI_RED, ANSI_WHITE, message, ANSI_RESET);
}

void printInfo(const char* message) {
    printf("%sℹ %s%s%s\n", ANSI_BLUE, ANSI_WHITE, message, ANSI_RESET);
}

void printTask(const char* task, int completed) {
    if (completed) {
        printf("%s%s[X] %s%s%s%s\n", ANSI_GRAY, ANSI_STRIKE, task, ANSI_RESET, ANSI_GRAY, ANSI_RESET);
    } else {
        printf("%s[ ] %s%s%s\n", ANSI_BLUE, ANSI_WHITE, task, ANSI_RESET);
    }
}

void renderTodoList(TodoList* todos, int selectedIndex, const char* currentListName) {
    clearScreen();
    
    printf("%s+-------------------------------------------------------------+%s\n", ANSI_BLUE, ANSI_RESET);
    printf("%s|%s%s                    TODO APPLICATION (%s)                     %s|%s\n", 
           ANSI_BLUE, ANSI_BOLD, ANSI_WHITE, currentListName, ANSI_BLUE, ANSI_RESET);
    printf("%s+-------------------------------------------------------------+%s\n", ANSI_BLUE, ANSI_RESET);
    printf("%s  Use UP/DOWN to navigate, SPACE to toggle, D to delete, I to add, ESC to exit%s\n",
           ANSI_WHITE, ANSI_RESET);
    printf("\n");
    
    if (!todos || todos->count == 0) {
        printf("%s  No todos yet. Press I to add one.%s\n", ANSI_WHITE, ANSI_RESET);
    } else {
        for (int i = 0; i < todos->count; i++) {
            Todo* todo = &todos->items[i];
            int isSelected = (i == selectedIndex);
            
            if (isSelected) {
                printf("%s%s> %s", ANSI_BG_BLUE, ANSI_WHITE, ANSI_RESET);
            } else {
                printf("%s  %s", ANSI_WHITE, ANSI_RESET);
            }
            
            printf("%d. ", i + 1);
            
            if (todo->completed) {
                printf("%s%s[X] %s%s%s%s\n", ANSI_GRAY, ANSI_STRIKE, todo->task, ANSI_RESET, ANSI_GRAY, ANSI_RESET);
            } else {
                printf("%s[ ] %s%s%s\n", ANSI_BLUE, ANSI_WHITE, todo->task, ANSI_RESET);
            }
        }
    }
    
    printf("\n");
    printf("%s+-------------------------------------------------------------+%s\n", ANSI_BLUE, ANSI_RESET);
    fflush(stdout);
}

void renderListSelection(char** lists, int listCount, int selectedIndex) {
    clearScreen();
    
    printf("%sSelect a list or create a new one:%s\n", ANSI_BLUE, ANSI_RESET);
    
    for (int i = 0; i < listCount; i++) {
        int isSelected = (i == selectedIndex);
        
        if (isSelected) {
            printf("%s%s> %s%s", ANSI_BG_BLUE, ANSI_WHITE, lists[i], ANSI_RESET);
        } else {
            printf("%s  %s%s", ANSI_WHITE, lists[i], ANSI_RESET);
        }
        printf("\n");
    }
    
    // Print "Create a new list" option
    int isSelected = (selectedIndex == listCount);
    if (isSelected) {
        printf("%s%s> Create a new list%s\n", ANSI_BG_BLUE, ANSI_WHITE, ANSI_RESET);
    } else {
        printf("%s  Create a new list%s\n", ANSI_WHITE, ANSI_RESET);
    }
    
    fflush(stdout);
}

char* formatTodoForDisplay(const Todo* todo, int index) {
    static char buffer[1024];
    
    if (todo->completed) {
        snprintf(buffer, sizeof(buffer), "%s%s%d. ✓ %s%s%s%s",
                ANSI_GRAY, ANSI_STRIKE, index + 1, todo->task, ANSI_RESET, ANSI_GRAY, ANSI_RESET);
    } else {
        snprintf(buffer, sizeof(buffer), "%s○ %s%d. %s%s",
                ANSI_BLUE, ANSI_WHITE, index + 1, todo->task, ANSI_RESET);
    }
    
    return buffer;
}
