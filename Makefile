CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./include -I./cJSON
CJSON_OBJ = cJSON/cJSON.o

# Note: This Makefile requires cJSON to be cloned first:
#   git clone https://github.com/DaveGamble/cJSON.git
#   cd cJSON && gcc -c -std=c99 cJSON.c -o cJSON.o && cd ..

SRCDIR = src
OBJDIR = build
BINDIR = build

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/todo-cli

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS) $(CJSON_OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(CJSON_OBJ)
	@echo "Build successful: $@"

# Ensure cJSON is compiled
$(CJSON_OBJ):
	@if not exist cJSON ( \
		echo Cloning cJSON library... && \
		git clone https://github.com/DaveGamble/cJSON.git \
	)
	@if not exist $(CJSON_OBJ) ( \
		echo Compiling cJSON... && \
		cd cJSON && gcc -c -std=c99 cJSON.c -o cJSON.o && cd .. \
	)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	$(TARGET)

clean:
	rm -rf $(OBJDIR)/*.o $(TARGET)

help:
	@echo "Available targets:"
	@echo "  make all   - Build the application"
	@echo "  make run   - Build and run the application"
	@echo "  make clean - Remove build artifacts"
