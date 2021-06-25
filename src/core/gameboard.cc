#include <core/gameboard.h>

namespace connect_four {

GameBoard::GameBoard() : is_x_turn_(true), pieces_(vector<vector<int>>()),
      gamestate_(BoardState::InProgress) {
  // Resize the vector
  pieces_.resize(kHeight, vector<int>(kWidth, 0));
}

GameBoard::GameBoard(const vector<vector<int>>& pieces, bool is_x_turn) :
      pieces_(pieces), is_x_turn_(is_x_turn), gamestate_(BoardState::InProgress) {
  // Check board validity
  if (pieces.size() != kHeight || pieces[0].size() != kWidth) {
    throw std::invalid_argument("Board is of wrong shape");
  }

  // Update and check gamestate
  if (!ArePiecesValid(kXPiece, kOPiece)) {
    throw std::invalid_argument("Board is invalid");
  }

  UpdateGameState();
}

void GameBoard::Reset() {
  for (vector<int>& row : pieces_) {
    std::fill(row.begin(), row.end(), 0);
  }
  gamestate_ = BoardState::InProgress;
  is_x_turn_ = true;
}

int GameBoard::GetPieceAtLocation(size_t row, size_t column) const {
  return pieces_[row][column];
}

bool GameBoard::GetIsXTurn() const {
  return is_x_turn_;
}

BoardState GameBoard::GetGameState() const {
  return gamestate_;
}

vector<size_t> GameBoard::CalculateValidColumns() const {
  vector<size_t> valids;

  // Check if the game is over, then check all columns
  if (gamestate_ != BoardState::InProgress) {
    return valids;
  }

  for (size_t col : kColumnOrder) {
    if (FindColumnBottom(col) != -1) {
      valids.push_back(col);
    }
  }
  return valids;
}

bool GameBoard::DropPiece(size_t column) {
  // Check out of range
  if (column >= kWidth) {
    throw std::out_of_range("Column out of range");
  }

  // Get the current player's piece
  int piece = kXPiece;
  if (!is_x_turn_) {
    piece = kOPiece;
  }

  int bottom = FindColumnBottom(column);
  if (gamestate_ != BoardState::InProgress || bottom == -1) {
    return false;
  }

  // Place the piece, update turn and gamestate
  pieces_[bottom][column] = piece;
  is_x_turn_ = !is_x_turn_;
  UpdateGameState();

  return true;
}

std::vector<float> GameBoard::GenerateVectorFeatures() const {
  std::vector<float> features;
  features.resize(42, 0);
  for (size_t index = 0; index < kWidth * kHeight; index++) {
    features[index] = GetPieceAtLocation(index / kWidth,
                                          index % kWidth);
  }
  return features;
}

bool GameBoard::IsColumnValid(size_t col) const {
  // An invalid column has a space followed by a piece
  for (size_t row = 0; row < kHeight - 1; row++) {
    if (GetPieceAtLocation(row + 1, col) == kEmpty &&
        GetPieceAtLocation(row, col) != kEmpty) {
      return false;
    }
  }
  return true;
}

int GameBoard::FindColumnBottom(size_t col) const {
  for (int row = kHeight; row > 0; row--) {
    if (GetPieceAtLocation(row - 1, col) == kEmpty) {
      return row - 1;
    }
  }
  return -1;
}

void GameBoard::UpdateGameState() {
  int x_piece = kXPiece;
  int o_piece = kOPiece;

  if (HasHorizontalWins(x_piece) || HasVerticalWins(x_piece) ||
      HasUpRightDiagonalWins(x_piece) || HasDownRightDiagonalWins(x_piece)) {
    gamestate_ = BoardState::Xwins;
    return;
  }

  if (HasHorizontalWins(o_piece) || HasVerticalWins(o_piece) ||
      HasUpRightDiagonalWins(o_piece) || HasDownRightDiagonalWins(o_piece)) {
    gamestate_ = BoardState::Owins;
    return;
  }

  // If each column is filled, the game is a tie
  bool all_filled = true;
  for (size_t col = 0; col < kWidth; col++) {
    if (FindColumnBottom(col) != -1) {
      all_filled = false;
      break;
    }
  }

  if (all_filled) {
    gamestate_ = BoardState::Tie;
    return;
  }

  // Otherwise the game is in progress
  gamestate_ = BoardState::InProgress;
}

bool GameBoard::ArePiecesValid(int x_piece, int o_piece) const {
  // Check column validity
  for (size_t col = 0; col < kWidth; col++) {
    if (!IsColumnValid(col)) {
      return false;
    }
  }

  // Check number of pieces for each player
  int x_pieces = CalculateNumberPieces(x_piece);
  int o_pieces = CalculateNumberPieces(o_piece);

  if (is_x_turn_ && (x_pieces != o_pieces) ||
      !is_x_turn_ && (x_pieces != o_pieces + 1)) {
    return false;
  }
  return true;
}

// Helper methods to check wins
bool GameBoard::HasVerticalWins(int piece) const {
  // Subtract 4 since 4 pieces must be connected
  for (size_t row = 0; row <= kHeight - 4; row++) {
    for (size_t col = 0; col < kWidth; col++) {
      if (GetPieceAtLocation(row, col) == piece &&
          GetPieceAtLocation(row + 1, col) == piece &&
          GetPieceAtLocation(row + 2, col) == piece &&
          GetPieceAtLocation(row + 3, col) == piece) {
        return true;
      }
    }
  }
  return false;
}

bool GameBoard::HasHorizontalWins(int piece) const {
  // Subtract 4 since 4 pieces must be connected
  for (size_t row = 0; row < kHeight; row++) {
    for (size_t col = 0; col <= kWidth - 4; col++) {
      if (GetPieceAtLocation(row, col) == piece &&
          GetPieceAtLocation(row, col + 1) == piece &&
          GetPieceAtLocation(row, col + 2) == piece &&
          GetPieceAtLocation(row, col + 3) == piece) {
        return true;
      }
    }
  }
  return false;
}

bool GameBoard::HasDownRightDiagonalWins(int piece) const {
  // Subtract 4 since 4 pieces must be connected
  for (size_t row = 0; row <= kHeight - 4; row++) {
    for (size_t col = 0; col <= kWidth - 4; col++) {
      if (GetPieceAtLocation(row, col) == piece &&
          GetPieceAtLocation(row + 1, col + 1) == piece &&
          GetPieceAtLocation(row + 2, col + 2) == piece &&
          GetPieceAtLocation(row + 3, col + 3) == piece) {
        return true;
      }
    }
  }
  return false;
}

bool GameBoard::HasUpRightDiagonalWins(int piece) const {
  // Subtract 4 since 4 pieces must be connected
  for (size_t row = 0; row <= kHeight - 4; row++) {
    for (size_t col = 0; col <= kWidth - 4; col++) {
      if (GetPieceAtLocation(row + 3, col) == piece &&
          GetPieceAtLocation(row + 2, col + 1) == piece &&
          GetPieceAtLocation(row + 1, col + 2) == piece &&
          GetPieceAtLocation(row, col + 3) == piece) {
        return true;
      }
    }
  }
  return false;
}

size_t GameBoard::CalculateNumberPieces(int piece) const {
  size_t sum = 0;
  for(const vector<int>& row: pieces_) {
    sum += count(row.begin(), row.end(), piece);
  }
  return sum;
}

} // namespace connect_four