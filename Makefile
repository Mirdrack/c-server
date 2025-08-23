CC ?= gcc
CFLAGS = -Wall -Wextra -g -pthread -Iinclude
LDFLAGS = -pthread
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
BIN = build/main

# Test sources
SRC_NO_MAIN = $(filter-out src/main.c, $(SRC))
TEST_SRC = $(wildcard tests/*.c)
TEST_BIN = build/run_tests

all: $(BIN)

$(BIN): $(OBJ)
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Build test runner
$(TEST_BIN): $(SRC_NO_MAIN) $(TEST_SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -lcheck -lm -lrt -lsubunit

# Run tests
test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -rf build src/*.o tests/*.o

dev:
	docker-compose run --rm dev

check:
	docker-compose run --rm check

run:
	docker-compose run --rm -p 8080:8080 dev ./build/main

.PHONY: all clean dev run test
