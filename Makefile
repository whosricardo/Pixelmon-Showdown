CC = gcc
SRC = src/main.c lib/cJSON.c
INCLUDES = -Iinclude -I/opt/homebrew/opt/raylib/include
LIBS = -L/opt/homebrew/opt/raylib/lib -lraylib \
       -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo

OUT = pixelmon_quest

all:
\t$(CC) $(SRC) $(INCLUDES) $(LIBS) -o $(OUT)

clean:
\trm -f $(OUT)
