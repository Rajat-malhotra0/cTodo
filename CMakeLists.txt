cmake_minimum_required(VERSION 3.10)
project(TodoCLI C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Find cJSON library (required dependency)
find_package(cjson CONFIG REQUIRED)

# Collect source files
set(SOURCES
    src/main.c
    src/todo.c
    src/storage.c
    src/ui.c
)

# Create executable
add_executable(todo-cli ${SOURCES})

# Link cJSON library
target_link_libraries(todo-cli PRIVATE cjson::cjson)

# Platform-specific settings
if(WIN32)
    # Windows specific
else()
    # Unix/Linux specific
    target_link_libraries(todo-cli PRIVATE m)  # Math library
endif()

# Installation
install(TARGETS todo-cli DESTINATION bin)
