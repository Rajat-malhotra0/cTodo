#ifndef UI_H
#define UI_H

#include "todo.h"

// Clear the screen
void clearScreen();

// Print header with text
void printHeader(const char* text);

// Print success message
void printSuccess(const char* message);

// Print error message
void printError(const char* message);

// Print info message
void printInfo(const char* message);

// Print a single task
void printTask(const char* task, int completed);

// Render the main todo list screen
void renderTodoList(TodoList* todos, int selectedIndex, const char* currentListName);

// Render list selection screen
void renderListSelection(char** lists, int listCount, int selectedIndex);

// Format a todo for display
char* formatTodoForDisplay(const Todo* todo, int index);

#endif // UI_H
