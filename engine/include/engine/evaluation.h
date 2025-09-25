#ifndef ENGINE_EVALUATION_H
#define ENGINE_EVALUATION_H

#include "engine/board.h"

typedef struct {
  int win_score;
  int opponent_win_score;
  int three_in_row_score;
  int opponent_three_in_row_score;
  int two_in_row_score;
  int opponent_two_in_row_score;
} EvaluationWeights;

extern const EvaluationWeights EVALUATION_DEFAULT_WEIGHTS;

typedef double (*HeuristicFn)(const Board *board, Player maximizer,
                              const EvaluationWeights *weights);

double evaluation_default(const Board *board, Player maximizer,
                          const EvaluationWeights *weights);

#endif // ENGINE_EVALUATION_H
