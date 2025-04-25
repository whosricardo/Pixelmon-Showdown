# Compiler and flags
CC = gcc
CFLAGS = -Iinclude -I/opt/homebrew/opt/raylib/include

# Linker flags
LDFLAGS = -L/opt/homebrew/opt/raylib/lib -lraylib \
  -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo

# Sources
SRC = src/player_animado.c lib/cJSON.c
OUT = player_animado

IDLE_SRC = src/player_animado_idle.c
IDLE_OUT = player_animado_idle

all: $(OUT) map_test

$(OUT): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LDFLAGS) -o $(OUT)

idle:
	$(CC) $(IDLE_SRC) lib/cJSON.c $(CFLAGS) $(LDFLAGS) -o $(IDLE_OUT)

test_loader: src/test_loader.c src/pokemon_loader.c src/file_loader.c lib/cJSON.c
	$(CC) src/test_loader.c src/pokemon_loader.c src/file_loader.c lib/cJSON.c -Iinclude $(CFLAGS) -o test_loader

map_test: src/map_test.c src/tile.c src/map.c
	$(CC) src/map_test.c src/tile.c src/map.c $(CFLAGS) $(LDFLAGS) -o map_test

clean:
	rm -f $(OUT) $(IDLE_OUT) test_loader map_test
