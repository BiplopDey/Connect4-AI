#include "engine/search.h"
#include "engine/config.h"

#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void default_move_order(const Board *board, int *columns, int count) {
  (void)board;
  const int center = N / 2;
  for (int i = 0; i < count - 1; i++) {
    for (int j = i + 1; j < count; j++) {
      int distance_i = abs(columns[i] - center);
      int distance_j = abs(columns[j] - center);
      if (distance_j < distance_i ||
          (distance_i == distance_j && columns[j] < columns[i])) {
        int tmp = columns[i];
        columns[i] = columns[j];
        columns[j] = tmp;
      }
    }
  }
}

static const EvaluationWeights *effective_weights(const SearchConfig *config) {
  return &config->weights;
}

static HeuristicFn effective_evaluator(const SearchConfig *config) {
  return config->evaluate ? config->evaluate : evaluation_default;
}

static double evaluate_position(const Board *board, const SearchConfig *config) {
  const EvaluationWeights *weights = effective_weights(config);
  return effective_evaluator(config)(board, config->maximizing_player, weights);
}

static MoveOrderFn effective_move_order(const SearchConfig *config) {
  return config->order_moves ? config->order_moves : default_move_order;
}

static Player opponent_of(Player player) {
  return player == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE;
}

static double min_value(const Board *board, const SearchConfig *config,
                        int depth, double alpha, double beta,
                        Player current_player);

static double max_value(const Board *board, const SearchConfig *config,
                        int depth, double alpha, double beta,
                        Player current_player) {
  int moves[N];
  int move_count = board_legal_moves(board, moves);

  if (depth >= config->max_depth || move_count == 0)
    return evaluate_position(board, config);

  effective_move_order(config)(board, moves, move_count);

  double value = -DBL_MAX;
  for (int i = 0; i < move_count; i++) {
    Board child;
    board_copy(&child, board);
    if (!board_drop_token(&child, moves[i], current_player, NULL))
      continue;

    double child_value =
        min_value(&child, config, depth + 1, alpha, beta,
                  opponent_of(current_player));
    if (child_value > value)
      value = child_value;

    if (value > alpha)
      alpha = value;

    if (alpha >= beta)
      break;
  }

  return value;
}

static double min_value(const Board *board, const SearchConfig *config,
                        int depth, double alpha, double beta,
                        Player current_player) {
  int moves[N];
  int move_count = board_legal_moves(board, moves);

  if (depth >= config->max_depth || move_count == 0)
    return evaluate_position(board, config);

  effective_move_order(config)(board, moves, move_count);

  double value = DBL_MAX;
  for (int i = 0; i < move_count; i++) {
    Board child;
    board_copy(&child, board);
    if (!board_drop_token(&child, moves[i], current_player, NULL))
      continue;

    double child_value =
        max_value(&child, config, depth + 1, alpha, beta,
                  opponent_of(current_player));
    if (child_value < value)
      value = child_value;

    if (value < beta)
      beta = value;

    if (alpha >= beta)
      break;
  }

  return value;
}

void search_config_init(SearchConfig *config) {
  if (!config)
    return;
  config->max_depth = ENGINE_DEFAULT_MAX_DEPTH;
  config->maximizing_player = PLAYER_TWO;
  config->evaluate = evaluation_default;
  config->order_moves = NULL;
  config->weights = EVALUATION_DEFAULT_WEIGHTS;
}

int search_best_move(const Board *board, Player to_move,
                     const SearchConfig *config) {
  if (!board || !config)
    return -1;

  int moves[N];
  int move_count = board_legal_moves(board, moves);
  if (move_count == 0)
    return -1;

  MoveOrderFn order = effective_move_order(config);
  order(board, moves, move_count);

  Player opponent = opponent_of(to_move);
  double best_value = (to_move == config->maximizing_player) ? -DBL_MAX : DBL_MAX;
  int best_column = moves[0];

  for (int i = 0; i < move_count; i++) {
    Board child;
    board_copy(&child, board);
    if (!board_drop_token(&child, moves[i], to_move, NULL))
      continue;

    double value;
    if (to_move == config->maximizing_player) {
      value = min_value(&child, config, 1, -DBL_MAX, DBL_MAX, opponent);
      if (value > best_value) {
        best_value = value;
        best_column = moves[i];
      }
    } else {
      value = max_value(&child, config, 1, -DBL_MAX, DBL_MAX, opponent);
      if (value < best_value) {
        best_value = value;
        best_column = moves[i];
      }
    }
  }

  return best_column;
}
