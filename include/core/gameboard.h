#pragma once

#include <vector>
#include <algorithm>

namespace connect_four {

using std::vector;

enum class BoardState {
  InProgress,
  Xwins,
  Owins,
  Tie,
};

/**
 * A gameboard for connect-four
 */
class GameBoard {
 public:
  // Constants for width and height
  const size_t kWidth = 7;
  const size_t kHeight = 6;

  // Constants for pieces corresponding to model inputs
  const int kEmpty = 0;
  const int kXPiece = 1;
  const int kOPiece = -1;

  /**
   * Construct an empty gameboard
   */
  GameBoard();

  /**
   * Construct a gameboard from a vector of pieces
   * @param pieces A 2D vector of pieces
   * @throw invalid_argument exception if pieces is of incorrect length,
   * or if the resulting board is obviously invalid (too many pieces of one
   * color, floating pieces).
   */
  GameBoard(const vector<vector<int>>& pieces, bool is_x_turn);

  /**
   * Calculate the valid columns for a player to place a piece in.
   * @return A vector with all the valid column indices to place in.
   * Returns an empty vector if the game is over.
   */
  vector<size_t> CalculateValidColumns() const;

  /**
   * Returns a boardstate enum corresponding to the state (InvalidState,
   * GameNotOver, XWins, OWins, GameTied)
   */
  BoardState GetGameState() const;

  /**
   * Gets the piece at a coordinate of the board.
   * @param row The zero-indexed y-coordinate of the piece
   * @param column The zero-indexed x-coordinate of the piece
   * @return The piece at the location represented by a constant
   * integer (kEmpty, kXPiece, kOPiece).
   * @throw out_of_range exception if row or column is outside of the board.
   */
  int GetPieceAtLocation(size_t row, size_t column) const;

  /**
   * Drops a piece in a column and returns true if the action was successful,
   * false if not.
   * @param column The zero-indexed column to drop a piece in
   * @return True if the action is successful, false otherwise.
   * @throw out_of_range exception if column is outside the board
   */
  bool DropPiece(size_t column);

  /**
   * Resets the board to the original state.
   */
  void Reset();

  // Getters
  bool GetIsXTurn() const;

  /**
   * Create a vector representation of the position to input to the model.
   * @return A vector containing one vec_t of length 42 representing the board.
   */
  std::vector<float> GenerateVectorFeatures() const;

 private:
  // A 2D vector storing integers representing pieces
  vector<vector<int>> pieces_;
  // The turn
  bool is_x_turn_;
  // Store the gamestate so it only has to be calculated each turn
  BoardState gamestate_;

  // Return valid columns from center to edge
  std::vector<size_t> kColumnOrder = {3, 4, 2, 5, 1, 6, 0};

  /**
   * Updates the gamestate for the current board position.
   * This method assumes the original gamestate was valid, so it does not
   * check for invalid positions.
   */
  void UpdateGameState();

  /**
   * Checks if the column is valid (there are no empty spaces below pieces)
   * @throw out_of_range exception if the column is out of range
   */
  bool IsColumnValid(size_t col) const;

  /**
   * Finds the bottom empty space of a column.
   * @return The index of the empty space, or -1 if the column is filled.
   * @throw out_of_range exception if the column is out of range
   */
  int FindColumnBottom(size_t col) const;

  /**
   * Checks if a board is valid based on the position of the pieces of
   * the two players and the number of pieces, not based on potential wins
   * @return A boolean of whether the board is valid based on this
   * initial check of validity.
   */
  bool ArePiecesValid(int x_piece, int o_piece) const;

  // Helpers to check for the existence of different types of wins based on the
  // type of piece
  bool HasVerticalWins(int piece) const;
  bool HasHorizontalWins(int piece) const;
  bool HasUpRightDiagonalWins(int piece) const;
  bool HasDownRightDiagonalWins(int piece) const;

  // Finds the number of matching pieces
  size_t CalculateNumberPieces(int piece) const;
};

} // namespace connect_four