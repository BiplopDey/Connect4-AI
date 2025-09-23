CC := clang
CFLAGS ?= -g -Wno-everything -pthread
LDFLAGS ?= -lm

CORE_SRCS := board.c evaluation.c search.c

.PHONY: all clean test

all: engine

# Headless engine that prints best AI column
engine: engine_cli.c $(CORE_SRCS)
	$(CC) $(CFLAGS) -DCONNECT4_DISABLE_CLI engine_cli.c $(CORE_SRCS) -o "$@" $(LDFLAGS)

search_tests: test_search.c $(CORE_SRCS)
	$(CC) $(CFLAGS) -DCONNECT4_DISABLE_CLI test_search.c $(CORE_SRCS) -o "$@" $(LDFLAGS)

board_tests: test_board.c board.c
	$(CC) $(CFLAGS) -DCONNECT4_DISABLE_CLI test_board.c board.c -o "$@" $(LDFLAGS)

test: search_tests board_tests
	./search_tests
	./board_tests

clean:
	rm -f engine search_tests board_tests
