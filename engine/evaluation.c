#include "evaluation.h"

#include <stddef.h>

static int check_line(Player a1, Player a2, Player a3, Player a4, Player player,
                      int length) {
  switch (length) {
  case 2:
    return ((a1 == player && a2 == player && a3 == PLAYER_NONE) +
            (a2 == player && a3 == player && a4 == PLAYER_NONE) +
            (a1 == PLAYER_NONE && a2 == player && a3 == player) +
            (a2 == PLAYER_NONE && a3 == player && a4 == player));
  case 3:
    return ((a1 == player && a2 == player && a3 == player &&
             a4 == PLAYER_NONE) +
            (a1 == PLAYER_NONE && a2 == player && a3 == player &&
             a4 == player));
  case 4:
    return (a1 == player && a2 == player && a3 == player && a4 == player);
  default:
    return 0;
  }
}

static int count_sequences(const Board *board, Player player, int length) {
  int total = 0;

  for (int row = 0; row <= N - 4; row++) {
    for (int col = 0; col < N; col++) {
      total += check_line(board->cells[row][col], board->cells[row + 1][col],
                          board->cells[row + 2][col],
                          board->cells[row + 3][col], player, length);
    }
  }

  for (int row = 0; row < N; row++) {
    for (int col = 0; col <= N - 4; col++) {
      total += check_line(board->cells[row][col],
                          board->cells[row][col + 1],
                          board->cells[row][col + 2],
                          board->cells[row][col + 3], player, length);
    }
  }

  for (int row = 0; row <= N - 4; row++) {
    for (int col = 0; col <= N - 4; col++) {
      total += check_line(board->cells[row][col],
                          board->cells[row + 1][col + 1],
                          board->cells[row + 2][col + 2],
                          board->cells[row + 3][col + 3], player, length);
    }
  }

  for (int row = 3; row < N; row++) {
    for (int col = 0; col <= N - 4; col++) {
      total += check_line(board->cells[row][col],
                          board->cells[row - 1][col + 1],
                          board->cells[row - 2][col + 2],
                          board->cells[row - 3][col + 3], player, length);
    }
  }

  return total;
}

const EvaluationWeights EVALUATION_DEFAULT_WEIGHTS = {
    .win_score = 100000,
    .opponent_win_score = -100000,
    .three_in_row_score = 10,
    .opponent_three_in_row_score = -10,
    .two_in_row_score = 2,
    .opponent_two_in_row_score = -2,
};

double evaluation_default(const Board *board, Player maximizer,
                          const EvaluationWeights *weights) {
  if (!board)
    return 0.0;

  const EvaluationWeights *w = weights ? weights : &EVALUATION_DEFAULT_WEIGHTS;
  Player opponent = maximizer == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE;

  if (count_sequences(board, maximizer, 4))
    return w->win_score;
  if (count_sequences(board, opponent, 4))
    return w->opponent_win_score;

  double score = 0.0;
  score += count_sequences(board, maximizer, 3) * w->three_in_row_score;
  score += count_sequences(board, opponent, 3) * w->opponent_three_in_row_score;
  score += count_sequences(board, maximizer, 2) * w->two_in_row_score;
  score += count_sequences(board, opponent, 2) * w->opponent_two_in_row_score;

  return score;
}
