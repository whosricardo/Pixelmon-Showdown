# Compiler and flags
CC = gcc
CFLAGS = -Iinclude -I/opt/homebrew/opt/raylib/include

# Linker flags
LDFLAGS = -L/opt/homebrew/opt/raylib/lib -lraylib \
  -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo

# Sources
SRC = src/player_animado.c lib/cJSON.c
OUT = player_animado

# Alternative: build idle version
IDLE_SRC = src/player_animado_idle.c
IDLE_OUT = player_animado_idle

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LDFLAGS) -o $(OUT)

idle:
	$(CC) $(IDLE_SRC) lib/cJSON.c $(CFLAGS) $(LDFLAGS) -o $(IDLE_OUT)

clean:
	rm -f $(OUT) $(IDLE_OUT)
