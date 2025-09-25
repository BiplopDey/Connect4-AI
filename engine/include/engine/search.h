#ifndef ENGINE_SEARCH_H
#define ENGINE_SEARCH_H

#include "engine/board.h"
#include "engine/evaluation.h"

typedef void (*MoveOrderFn)(const Board *board, int *columns, int count);

typedef struct {
  int max_depth;
  Player maximizing_player;
  HeuristicFn evaluate;
  MoveOrderFn order_moves;
  EvaluationWeights weights;
} SearchConfig;

void search_config_init(SearchConfig *config);
int search_best_move(const Board *board, Player to_move,
                     const SearchConfig *config);

#endif // ENGINE_SEARCH_H
