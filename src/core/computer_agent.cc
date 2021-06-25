#include <core/computer_agent.h>

namespace connect_four {

Computer::Computer() {
  model_.load("net_2");
}

float Computer::FloatEvaluateBoard(const GameBoard &board,
                                   bool is_x_perspective) {
  tiny_dnn::vec_t evaluation = model_.predict(board.GenerateVectorFeatures());
  // The score from red's perspective is chance of winning minus chance of losing
  float red_score = evaluation[2] - evaluation[0];

  // If it's yellow's turn, flip the score
  if (!is_x_perspective) {
    return -1 * red_score;
  }
  return red_score;
}

tiny_dnn::vec_t Computer::VectorEvaluateBoard(const GameBoard &board) {
  return model_.predict(board.GenerateVectorFeatures());
}

// Reference: https://github.com/KeithGalli/Connect4-Python/blob/master/connect4_with_ai.py
move_evaluation_pair Computer::MiniMaxSearch(const GameBoard &board,
                                             size_t depth,
                                             float alpha, float beta,
                                             bool is_computer_x,
                                             bool maximizing_player) {

  BoardState state = board.GetGameState();

  // Check if the game is over
  if (state == BoardState::Xwins) {
    if (is_computer_x) {
      return {0, kWinLossValue};
    }
    return {0, -kWinLossValue};
  } else if (state == BoardState::Owins) {
    if (is_computer_x) {
      return {0, -kWinLossValue};
    }
    return {0, kWinLossValue};
  } else if (state == BoardState::Tie) {
    return {0, 0};
  }

  // Check if depth is zero
  if (depth == 0) {
    return {0, FloatEvaluateBoard(board, is_computer_x)};
  }

  std::vector<size_t> valid_moves = board.CalculateValidColumns();

  if (maximizing_player) {
    float value = -100;
    size_t column = 0;

    for (size_t col : valid_moves) {
      GameBoard copy = board;
      copy.DropPiece(col);

      // Create a recursive search with one less depth and flipping the
      // maximizing player
      float new_score = MiniMaxSearch(copy, depth - 1, alpha, beta,
                                      is_computer_x, false).score;

      if (new_score > value) {
        value = new_score;
        column = col;
        alpha = std::max(alpha, value);

        // Alpha beta pruning
        if (alpha >= beta) {
          break;
        }
      }
    }
    return {column, value};
  } else {
    float value = 100;
    size_t column = 0;

    for (size_t col : valid_moves) {
      GameBoard copy = board;
      copy.DropPiece(col);

      // Create a recursive search with one less depth and flipping the
      // maximizing player
      float new_score = MiniMaxSearch(copy, depth - 1, alpha, beta,
                                      is_computer_x, true).score;

      if (new_score < value) {
        value = new_score;
        column = col;
        beta = std::min(beta, value);

        // Alpha beta pruning
        if (alpha >= beta) {
          break;
        }
      }
    }
    return {column, value};
  }
}

} // namespace connect_four

