CC=gcc
CFLAGS=-Iinclude -Wall -Wextra
LDFLAGS=-lraylib -lm
SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)
OUT=pokemon

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f src/*.o $(OUT)
