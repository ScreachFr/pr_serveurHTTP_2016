TARGET = bin/test

CC = gcc -fdiagnostics-color=auto
CFLAGS = -std=gnu99 -Wall -Werror -Wextra -O2 -Wno-incompatible-pointer-types -Wno-unused-value
LDFLAGS = -Wno-incompatible-pointer-types

OBJECTS = $(patsubst src/%.c, bin/%.o, $(wildcard src/*.c))
HEADERS = $(wildcard include/*.h)


all: mkdir $(TARGET)

debug: CFLAGS = -std=gnu99 -Wall -Wextra -O0 -ggdb3 -Wno-incompatible-pointer-types
debug: LDFLAGS += -fsanitize=undefined
debug: LDFLAGS += -fsanitize=address
debug: all


mkdir:
	mkdir -p bin

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

bin/%.o: src/%.c $(HEADERS)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

clean:
	-rm -rf bin
