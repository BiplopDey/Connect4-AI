CC := clang
CFLAGS ?= -g -Wno-everything -pthread
LDFLAGS ?= -lm

CORE_SRCS := connect4.c minimax.c

.PHONY: all clean test minimax_tests

all: engine

# Headless engine that prints best AI column
engine: engine_cli.c $(CORE_SRCS)
	$(CC) $(CFLAGS) -DCONNECT4_DISABLE_CLI engine_cli.c $(CORE_SRCS) -o "$@" $(LDFLAGS)

minimax_tests: test_minimax.c $(CORE_SRCS)
	$(CC) $(CFLAGS) -DCONNECT4_DISABLE_CLI test_minimax.c $(CORE_SRCS) -o "$@" $(LDFLAGS)

connect4_tests: test_connect4.c connect4.c
	$(CC) $(CFLAGS) -DCONNECT4_DISABLE_CLI test_connect4.c connect4.c -o "$@" $(LDFLAGS)

test: minimax_tests connect4_tests
	./minimax_tests
	./connect4_tests

clean:
	rm -f engine minimax_tests connect4_tests
