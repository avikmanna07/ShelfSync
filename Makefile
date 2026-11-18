# ShelfSync - Smart Library Management System
# Makefile for building the project (Windows-friendly)

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -I./src -O3 -flto -s
LDFLAGS = -s -flto
SRC_DIR = src
BUILD_DIR = build
TARGET = ShelfSync.exe

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Default target (optimized release build with LTO)
all: $(TARGET)

# Link executable (stripped)
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)
	@echo Build successful! Run with: .\$(TARGET)

# Compile source files (optimized)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts (Windows-compatible)
clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	@if exist $(TARGET) del /f /q $(TARGET)
	@echo Cleaned build artifacts.

# Run the program
run: $(TARGET)
	.\$(TARGET)

# Debug build with symbols (no optimization, no strip)
debug: CFLAGS = -std=c11 -Wall -Wextra -Werror -I./src -g -O0
debug: LDFLAGS =
debug: clean $(TARGET)

# Release build with max optimizations + LTO
release: CFLAGS = -std=c11 -Wall -Wextra -Werror -I./src -O3 -s -flto
release: LDFLAGS = -s -flto
release: clean $(TARGET)

# Size check
size: $(TARGET)
	@echo === Executable Size ===
	@dir $(TARGET)
	@echo.
	@echo === Section Sizes ===
	@size $(TARGET)

# Check for common issues
check:
	$(CC) $(CFLAGS) -fsyntax-only $(SRCS)

# Format code (requires clang-format)
format:
	clang-format -i $(SRC_DIR)/*.c $(SRC_DIR)/*.h

# Help
help:
	@echo Available targets:
	@echo   all      - Build optimized release with LTO (default)
	@echo   clean    - Remove build artifacts
	@echo   run      - Build and run the program
	@echo   debug    - Build with debug symbols (no optimization)
	@echo   release  - Build with max optimizations + LTO
	@echo   size     - Show executable size breakdown
	@echo   check    - Syntax check only
	@echo   format   - Format source code
	@echo   help     - Show this help

.PHONY: all clean run debug release size check format help