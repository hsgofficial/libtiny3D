CC = gcc
CFLAGS = -Wall -O2 -Iinclude

SRC = src/canvas.c src/math3d.c src/renderer.c src/lighting.c
OBJ = $(SRC:.c=.o)
LIB = build/libtiny3d.a

all: $(LIB) demo/test_math

$(LIB): $(OBJ)
	mkdir -p build
	ar rcs $(LIB) $(OBJ)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

demo/test_math: tests/test_math.c $(LIB)
	$(CC) $(CFLAGS) $< -Lbuild -ltiny3d -lm -o $@

clean:
	rm -f src/*.o build/*.a demo/test_math
