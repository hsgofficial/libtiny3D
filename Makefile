# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude
LDFLAGS = -lm

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = demo

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))
LIB = $(BUILD_DIR)/libtiny3d.a

# Detect OS for cross-platform directory creation
ifeq ($(OS),Windows_NT)
    MKDIR = if not exist $(1) mkdir $(1)
    RM = del /Q
    SLASH = \\
else
    MKDIR = mkdir -p $(1)
    RM = rm -rf
    SLASH = /
endif

.PHONY: all dirs clean run test

all: dirs $(LIB) $(BIN_DIR)/demo

# Cross-platform directory creation
dirs:
	@$(call MKDIR,build)
	@$(call MKDIR,demo)

# Build static library
$(LIB): $(OBJ)
	ar rcs $@ $^

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build demo executable
$(BIN_DIR)/demo: demo/main.c $(LIB)
	$(CC) $(CFLAGS) $< -L$(BUILD_DIR) -ltiny3d $(LDFLAGS) -o $@

# Clean build and bin directories
clean:
ifeq ($(OS),Windows_NT)
	-$(RM) $(BUILD_DIR)\*.o $(BUILD_DIR)\*.a
	-$(RM) $(BIN_DIR)\demo.exe
else
	-$(RM) $(BUILD_DIR)/*.o $(BUILD_DIR)/*.a
	-$(RM) $(BIN_DIR)/demo
endif

# Run the demo
run: all
	@$(BIN_DIR)/demo

# Run tests (if you have test executables)
test: all
	@echo "No test target implemented. Add your test targets here."

