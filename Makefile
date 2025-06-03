CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = src/canvas.c
OBJ = $(SRC:.c=.o)

DEMO = demo/main
DEMO_SRC = demo/main.c
DEMO_OBJ = $(DEMO_SRC:.c=.o)

BUILD_DIR = build

all: $(BUILD_DIR)/libtiny3d.a $(DEMO)

# Create static library
$(BUILD_DIR)/libtiny3d.a: $(OBJ)
	mkdir -p $(BUILD_DIR)
	ar rcs $@ $^

# Build demo
$(DEMO): $(DEMO_SRC) $(BUILD_DIR)/libtiny3d.a
	$(CC) $(CFLAGS) -o $@ $^ -lm

clean:
	rm -rf $(BUILD_DIR) demo/main *.o src/*.o

.PHONY: all clean
