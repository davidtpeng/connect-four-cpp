#include <visualizer/connect_four_app.h>

namespace connect_four {

namespace visualizer {

// board size = kWindowSize - 2 * kMargin
// top left is kMargin, kMargin
ConnectFourApp::ConnectFourApp() : board_(), evaluation_(0), is_player_x_(true),
                                    mouse_position_(0, 0) {
  // Set the window size and initialize with random particles
  ci::app::setWindowSize(static_cast<int>(kWindowSize),
                         static_cast<int>(kWindowSize));
}

void ConnectFourApp::draw() {
  ci::Color8u background_color(0, 0, 0);  // black
  ci::gl::clear(background_color);

  std::string result_text = GenerateGameStateText();
  ci::gl::drawStringCentered(
      result_text,
      glm::vec2((kWindowSize) / 2, kMargin / 4),
      ci::Color("white"),
      ci::Font("Arial", 24));

  std::string evaluation_text = GenerateEvaluationText(3);
  ci::gl::drawStringCentered(
      evaluation_text,
      glm::vec2((kWindowSize) / 2, kMargin / 2),
      ci::Color("white"),
      ci::Font("Arial", 24));

  DrawBoard();
}

void ConnectFourApp::update() {

}

void ConnectFourApp::mouseDown(ci::app::MouseEvent event) {
  if (board_.GetIsXTurn() == is_player_x_) {
    glm::vec2 position = event.getPos();
    int column = (position[0] - kMargin) / (2 * kPieceRadius);
    if (column < 0) {
      column = 0;
    } else if (column >= board_.kWidth) {
      column = board_.kWidth - 1;
    }
    
    board_.DropPiece(column);

    // Now the computer makes a move
    move_evaluation_pair best = model_.MiniMaxSearch(board_, depth_,
                                                     -model_.kAlphaBeta,
                                                     model_.kAlphaBeta,
                                                     !is_player_x_,
                                                     true);
    board_.DropPiece(best.column);

    // Update the evaluation
    evaluation_ = best.score;
  }
}

void ConnectFourApp::mouseMove(ci::app::MouseEvent event) {
  mouse_position_ = event.getPos();
}

void ConnectFourApp::DrawBoard() {
  ci::gl::color(ci::Color("white"));
  ci::Rectf pixel_bounding_box(glm::vec2(kMargin, kMargin),
                               glm::vec2(kWindowSize - kMargin,
                                         kWindowSize - kMargin - 100));
  ci::gl::drawStrokedRect(pixel_bounding_box);

  for (size_t row = 0; row < board_.kHeight; row++) {
    for (size_t col = 0; col < board_.kWidth; col++) {
      // Top left corner at (kMargin, kMargin)
      // Then offset by the number of radii moved
      glm::vec2 position(kMargin + kPieceRadius + 2 * kPieceRadius * col,
                         kMargin + kPieceRadius + 2 * kPieceRadius * row);

      // Switch the color based on the piece, then draw the circle
      int piece = board_.GetPieceAtLocation(row, col);
      if (piece == board_.kXPiece) {
        ci::gl::color(ci::Color("red"));
        ci::gl::drawSolidCircle(position, kPieceRadius);
      } else if (piece == board_.kOPiece) {
        ci::gl::color(ci::Color("yellow"));
        ci::gl::drawSolidCircle(position, kPieceRadius);
      } else {
        ci::gl::color(ci::Color("white"));
        ci::gl::drawStrokedCircle(position, kPieceRadius);
      }
    }
  }

  // Draw a circle indicating where the next move will be played
  int column = (mouse_position_[0] - kMargin) / (2 * kPieceRadius);
  if (column < 0) {
    column = 0;
  } else if (column >= board_.kWidth) {
    column = board_.kWidth - 1;
  }

  glm::vec2 position(kMargin + kPieceRadius + 2 * kPieceRadius * column,
                     kMargin + kPieceRadius + 2 * kPieceRadius * board_.kHeight);
  ci::gl::color(ci::Color("white"));
  ci::gl::drawStrokedCircle(position, kPieceRadius);
}

void ConnectFourApp::keyDown(ci::app::KeyEvent event) {
  switch (event.getCode()) {
      // Reset the board with player as first player
    case ci::app::KeyEvent::KEY_1:
      board_.Reset();
      evaluation_ = 0;
      is_player_x_ = true;
      break;
    case ci::app::KeyEvent::KEY_2:
      board_.Reset();
      is_player_x_ = false;
      // Computer should make a first move
      move_evaluation_pair best = model_.MiniMaxSearch(board_, 1,
                                                       -model_.kAlphaBeta,
                                                       model_.kAlphaBeta,
                                                       !is_player_x_,
                                                       true);
      board_.DropPiece(best.column);

      // Update the evaluation
      evaluation_ = best.score;
  }
}

std::string ConnectFourApp::GenerateGameStateText() const {
  std::string result_text;

  switch(board_.GetGameState()) {
    case BoardState::InProgress:
      result_text = "Game in progress...";
      break;
    case BoardState::Xwins:
      result_text = "Game over! Red has won.";
      break;
    case BoardState::Owins:
      result_text = "Game over! Yellow has won.";
      break;
    case BoardState::Tie:
      result_text = "Game over! It's a tie";
      break;
    default:
      result_text = "Invalid board state or other error";
  }
  result_text = result_text + " [Press 1 to play as red, 2 to play as yellow]";
  return result_text;
}

std::string ConnectFourApp::GenerateEvaluationText(size_t digits) {
  std::string evaluation_text = "Value: " +
                                TruncateDouble(evaluation_, digits);
  return evaluation_text;
}

// https://stackoverflow.com/questions/29200635/convert-float-to-string-with-precision-number-of-decimal-digits-specified
std::string ConnectFourApp::TruncateDouble(double number, size_t digits) const {
  std::stringstream stream;
  stream << std::fixed << std::setprecision(digits) << number;
  return stream.str();
}

}  // namespace visualizer

}  // namespace connect_four