# Compiler and flags
CC = gcc
SRC_DIR = src
LIB_DIR = lib
INCLUDE_DIR = include

SRC = $(SRC_DIR)/main.c \
      $(SRC_DIR)/menu.c \
      $(SRC_DIR)/team.c \
      $(SRC_DIR)/team_select.c \
      $(SRC_DIR)/rival_loader.c \
      $(SRC_DIR)/battle_screen.c \
      $(SRC_DIR)/pokemon_loader.c \
      $(SRC_DIR)/file_loader.c \
      $(SRC_DIR)/player_loader.c \
      $(LIB_DIR)/cJSON.c

OUT = pixelmon_showdown
TEST_SRC = $(SRC_DIR)/test_loader.c $(SRC_DIR)/pokemon_loader.c $(SRC_DIR)/file_loader.c $(LIB_DIR)/cJSON.c
TEST_OUT = test_loader
IDLE_SRC = $(SRC_DIR)/player_animado_idle.c $(LIB_DIR)/cJSON.c
IDLE_OUT = player_animado_idle

# Detect OS
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    CFLAGS = -Iinclude
    LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

ifeq ($(UNAME_S),Darwin)
    CFLAGS = -Iinclude -I/opt/homebrew/opt/raylib/include -Wall -Wextra -g
    LDFLAGS = -L/opt/homebrew/opt/raylib/lib -lraylib \
      -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo
endif

# Build targets
all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LDFLAGS) -o $(OUT)

idle: $(IDLE_SRC)
	$(CC) $(IDLE_SRC) $(CFLAGS) $(LDFLAGS) -o $(IDLE_OUT)

test_loader: $(TEST_SRC)
	$(CC) $(TEST_SRC) $(CFLAGS) $(LDFLAGS) -o $(TEST_OUT)

clean:
	rm -f $(OUT) $(IDLE_OUT) $(TEST_OUT)
