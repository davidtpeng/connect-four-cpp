#pragma once

#include <core/gameboard.h>

#include "tiny_dnn/tiny_dnn.h"

namespace connect_four {

// A struct storing a column to move in as well as its evaluation
struct move_evaluation_pair {
  size_t column;
  float score;

  move_evaluation_pair(size_t col, float val) : column(col), score(val) {};
};

/**
 * A computer model to evaluate connect four positions and suggest the
 * best moves using the minimax search algorithm with alpha-beta pruning.
 */
class Computer {
 public:
  // Constants for MiniMax Search
  // Set above 1 so guaranteed win overrides network evaluation
  const float kWinLossValue = 10;
  // Set above WinLossValue
  const float kAlphaBeta = 100;

  /**
   * Loads the default model.
   */
  Computer();

  /**
   * Gives an evaluation from -1 to 1 from the specified player's perspective.
   * Ex: If it is yellow to move and the score is 1, yellow is certain to win
   * Ex: If it is red to move and the score is 1, red is certain to win.
   * @param board A constant board reference
   * @return A float value from -1 to 1 representing the evaluation.
   */
  float FloatEvaluateBoard(const GameBoard& board,
                           bool is_x_perspective);

  /**
   * Gives the loss draw win probabilities of a position.
   */
  tiny_dnn::vec_t VectorEvaluateBoard(const GameBoard& board);

  /**
   * Given a board, use minimax search to find the best move.
   * @param board A constant board reference, the depth, alpha, beta,
   * a boolean of whether the computer is playing as x, and maximizing_player
   * which should be set to true.
   * @return A move evaluation pair with the best move and evaluation of that
   * move.
   * Returns a move evaluation pair with a move of 0 if no moves exist.
   */
  move_evaluation_pair MiniMaxSearch(const GameBoard& board,
                                     size_t depth,
                                     float alpha, float beta,
                                     bool is_computer_x,
                                     bool maximizing_player);

 private:
  tiny_dnn::network<tiny_dnn::sequential> model_;
};

} // namespace connect_four
