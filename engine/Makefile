CC := clang
CFLAGS ?= -g -Wno-everything -pthread
CPPFLAGS += -Iinclude
LDFLAGS ?= -lm

SRC_DIR := src
TEST_DIR := tests

BOARD_SRC := $(SRC_DIR)/board/board.c
EVALUATION_SRC := $(SRC_DIR)/evaluation/evaluation.c
SEARCH_SRC := $(SRC_DIR)/search/search.c
ENGINE_SRC := $(SRC_DIR)/cli/engine_cli.c

ENGINE_SRCS := $(BOARD_SRC) $(EVALUATION_SRC) $(SEARCH_SRC)

.PHONY: all clean test

all: engine

# Headless engine that prints best AI column
engine: $(ENGINE_SRC) $(ENGINE_SRCS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DCONNECT4_DISABLE_CLI $(ENGINE_SRC) $(ENGINE_SRCS) -o "$@" $(LDFLAGS)

search_tests: $(TEST_DIR)/test_search.c $(ENGINE_SRCS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DCONNECT4_DISABLE_CLI $^ -o "$@" $(LDFLAGS)

board_tests: $(TEST_DIR)/test_board.c $(BOARD_SRC)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DCONNECT4_DISABLE_CLI $^ -o "$@" $(LDFLAGS)

evaluation_tests: $(TEST_DIR)/test_evaluation.c $(BOARD_SRC) $(EVALUATION_SRC)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DCONNECT4_DISABLE_CLI $^ -o "$@" $(LDFLAGS)

test: search_tests board_tests evaluation_tests
	./search_tests
	./board_tests
	./evaluation_tests

clean:
	rm -f engine search_tests board_tests evaluation_tests
