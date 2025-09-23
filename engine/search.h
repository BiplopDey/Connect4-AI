#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"
#include "evaluation.h"

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

#endif // SEARCH_H
