#ifndef TODO_H
#define TODO_H

#include <time.h>

typedef struct {
    long long id;
    char* task;
    int completed;
    char* createdAt;
} Todo;

typedef struct {
    Todo* items;
    int count;
    int capacity;
} TodoList;

// Load all data from storage
void loadData();

// Get list of all available list names
char** getLists(int* count);

// Free list of names
void freeLists(char** lists, int count);

// Set current list to work with
int setCurrentList(const char* listName);

// Create a new list
int createList(const char* listName);

// Add a new todo
Todo* addTodo(const char* task);

// Get all todos (or filtered by completion status)
TodoList* listTodos(int showCompleted);

// Free todo list
void freeTodoList(TodoList* list);

// Toggle todo completion status
int completeTodo(int index);

// Delete todo at index
int deleteTodo(int index);

// Save all changes to storage
void saveData();

// Cleanup all data
void cleanupTodoData();

#endif // TODO_H
