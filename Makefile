# ShelfSync - Smart Library Management System
# Makefile for building the project

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -I./src -O2 -s
LDFLAGS = -s
SRC_DIR = src
BUILD_DIR = build
TARGET = ShelfSync

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Default target (optimized release build)
all: $(TARGET)

# Link executable (stripped)
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)
	@echo "Build successful! Run with: ./$(TARGET)"

# Compile source files (optimized)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	@echo "Cleaned build artifacts."

# Run the program
run: $(TARGET)
	./$(TARGET)

# Debug build with symbols (no optimization, no strip)
debug: CFLAGS = -std=c11 -Wall -Wextra -Werror -I./src -g -O0
debug: LDFLAGS =
debug: clean $(TARGET)

# Release build with max optimizations
release: CFLAGS = -std=c11 -Wall -Wextra -Werror -I./src -O3 -s -flto
release: LDFLAGS = -s -flto
release: clean $(TARGET)

# Size check
size: $(TARGET)
	@echo "=== Executable Size ==="
	@ls -lh $(TARGET)
	@echo ""
	@echo "=== Section Sizes ==="
	@size $(TARGET)

# Install (copy to /usr/local/bin on Unix-like systems)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Uninstall
uninstall:
	rm -f /usr/local/bin/$(TARGET)

# Check for common issues
check:
	$(CC) $(CFLAGS) -fsyntax-only $(SRCS)

# Format code (requires clang-format)
format:
	clang-format -i $(SRC_DIR)/*.c $(SRC_DIR)/*.h

# Help
help:
	@echo "Available targets:"
	@echo "  all      - Build optimized release (default)"
	@echo "  clean    - Remove build artifacts"
	@echo "  run      - Build and run the program"
	@echo "  debug    - Build with debug symbols (no optimization)"
	@echo "  release  - Build with max optimizations + LTO"
	@echo "  size     - Show executable size breakdown"
	@echo "  check    - Syntax check only"
	@echo "  format   - Format source code"
	@echo "  help     - Show this help"

.PHONY: all clean run debug release size check format help install uninstall