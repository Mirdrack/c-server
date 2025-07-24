CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
BIN = build/main

all: $(BIN)

$(BIN): $(OBJ)
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf build src/*.o

dev:
	docker-compose run --rm dev

run:
	docker-compose run --rm -p 8080:8080 dev ./build/main

.PHONY: all clean
