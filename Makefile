CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O2 -Iinclude
LDFLAGS = -lm
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = demo

# Source files
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))
LIB = $(BUILD_DIR)/libtiny3d.a

# Targets
all: dirs $(LIB) $(BIN_DIR)/demo

dirs:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

$(LIB): $(OBJ)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/demo: demo/main.c $(LIB)
	$(CC) $(CFLAGS) $< -L$(BUILD_DIR) -ltiny3d $(LDFLAGS) -o $@

clean:
ifeq ($(OS),Windows_NT)
	@del /Q $(BUILD_DIR)\*.* $(BIN_DIR)\*.*
else
	@rm -rf $(BUILD_DIR)/* $(BIN_DIR)/*
endif

run: all
	@$(BIN_DIR)/demo

test: all
	@echo "Running tests..."
	@$(BIN_DIR)/demo

.PHONY: all dirs clean run test
