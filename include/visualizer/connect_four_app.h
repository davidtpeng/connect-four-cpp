#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "tiny_dnn/tiny_dnn.h"

#include <core/gameboard.h>
#include <core/computer_agent.h>

#include <sstream>
#include <numeric>

namespace connect_four {

namespace visualizer {

/**
 * Simulates a connect four game and allows the user to play connect four
 */
class ConnectFourApp : public ci::app::App {
 public:
  ConnectFourApp();

  void draw() override;
  void update() override;

  // Track the mouse
  void mouseMove(ci::app::MouseEvent event) override;

  /**
   * Pressing delete resets the board
   * More key functionality to be added later
   */
  void keyDown(ci::app::KeyEvent event) override;

  void mouseDown(ci::app::MouseEvent event) override;

  // Constants for visuals
  const float kWindowSize = 900;
  const float kMargin = 100;
  const float kPieceRadius = 50;

 private:
  GameBoard board_;
  Computer model_;

  // Game settings
  bool is_player_x_;
  // The model plays stronger when this is set higher, but takes exponentially
  // more time
  size_t depth_ = 6;

  // Model prediction of evaluation, stored so it doesn't need to be
  // regenerated each frame
  float evaluation_;

  // Track the mouse
  glm::vec2 mouse_position_;

  /**
   * Draws the current board
   */
  void DrawBoard();

  /**
   * Create display text based on the state of the game (in progress,
   * first player wins, second player wins, tie)
   * @return A const reference to a string to be displayed on screen
   */
  std::string GenerateGameStateText() const;

  /**
   * Create display text based on the model's evaluation of the position
   * @param digits The number of digits to be displayed per number
   * @return A const reference to a string to be displayed on screen
   */
  std::string GenerateEvaluationText(size_t digits);

  /**
   * For printing a double to string, truncate to a specified number of digits.
   */
  std::string TruncateDouble(double number, size_t digits) const;
};

}  // namespace visualizer

}  // namespace connect_four
