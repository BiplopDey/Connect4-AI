CC := clang
CFLAGS ?= -g -Wno-everything -pthread
LDFLAGS ?= -lm

CORE_SRCS := connect4.c minimax.c

.PHONY: all clean test minimax_tests

all: main engine

# Interactive CLI game (uses main() in connect4.c)
main: $(CORE_SRCS)
	$(CC) $(CFLAGS) $(CORE_SRCS) -o "$@" $(LDFLAGS)

# Headless engine that prints best AI column
engine: engine_cli.c $(CORE_SRCS)
	$(CC) $(CFLAGS) -DCONNECT4_DISABLE_CLI engine_cli.c $(CORE_SRCS) -o "$@" $(LDFLAGS)

minimax_tests: test_minimax.c $(CORE_SRCS)
	$(CC) $(CFLAGS) -DCONNECT4_DISABLE_CLI test_minimax.c $(CORE_SRCS) -o "$@" $(LDFLAGS)

test: minimax_tests
	./minimax_tests

main-debug: $(CORE_SRCS)
	$(CC) $(CFLAGS) -O0 $(CORE_SRCS) -o "$@" $(LDFLAGS)

clean:
	rm -f main main-debug engine minimax_tests
