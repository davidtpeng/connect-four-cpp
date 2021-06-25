#include <catch2/catch.hpp>

#include <core/gameboard.h>

using connect_four::GameBoard;
using connect_four::BoardState;
using std::vector;

TEST_CASE("Default Constructor is valid and has no pieces") {
  GameBoard empty;
  REQUIRE(empty.GetIsXTurn());

  for (size_t row = 0; row < empty.kHeight; row++) {
    for (size_t col = 0; col < empty.kWidth; col++) {
      REQUIRE(empty.GetPieceAtLocation(row, col) == empty.kEmpty);
    }
  }
}

TEST_CASE("Cannot construct a GameBoard with an invalid board") {
  SECTION("Can't construct using a board with too many X pieces") {
    vector<vector<int>> invalid = { {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {1, 0, -1, 1, 0, -1, 1}};
    REQUIRE_THROWS_AS(GameBoard(invalid, true),
                      std::invalid_argument);

    vector<vector<int>> invalid_two = { {0, 0, 0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0, 0, 0},
                                        {1, 0, 0, 0, 0, 0, 1}};
    REQUIRE_THROWS_AS(GameBoard(invalid_two, false),
                      std::invalid_argument);
  }

  SECTION("Can't construct using a board with too many O pieces") {
    vector<vector<int>> invalid = { {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, -1, 0, 0, 0},
                                    {1, 0, -1, 1, 0, -1, 1}};
    REQUIRE_THROWS_AS(GameBoard(invalid, false),
                      std::invalid_argument);

    vector<vector<int>> invalid_two = { {0, 0, 0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0, 0, 0},
                                        {-1, 0, 0, 0, 0, 0, 0},
                                        {-1, 0, 0, 0, 0, 0, 1}};
    REQUIRE_THROWS_AS(GameBoard(invalid_two, true),
                      std::invalid_argument);
  }

  SECTION("Can't construct a board with floating pieces") {
    vector<vector<int>> invalid = { {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 1, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, -1, 0, 1, 0, 0, 0}};
    REQUIRE_THROWS_AS(GameBoard(invalid, false),
                      std::invalid_argument);
  }

  SECTION("Can't construct a board with wrong dimensions") {
    vector<vector<int>> invalid = { {0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0},
                                    {0, -1, 0, 1, 0, 0}};
    REQUIRE_THROWS_AS(GameBoard(invalid, true),
                      std::invalid_argument);
  }
}

TEST_CASE("2D Vector Constructor constructs with correct values") {
  vector<vector<int>> valid = {   {0, 0, 0, 0, 0, 0, 0},
                                  {0, 0, 0, 0, 0, 0, 0},
                                  {0, 0, 0, 0, 0, 0, 0},
                                  {0, 0, 0, 0, 0, 0, 0},
                                  {1, 0, 0, 0, 0, 0, 0},
                                  {1, 0, -1, 1, 0, -1, 0}};
  GameBoard test(valid, false);
  REQUIRE(test.GetIsXTurn() == false);

  // Check all pieces
  for (size_t row = 0; row < test.kHeight; row++) {
    for (size_t col = 0; col < test.kWidth; col++) {
      REQUIRE(test.GetPieceAtLocation(row, col) == valid[row][col]);
    }
  }
}

TEST_CASE("Reset Board") {
  GameBoard test;

  SECTION("Board is cleared and it's X's turn") {
    test.DropPiece(6);
    test.DropPiece(2);
    test.DropPiece(2);

    vector<vector<int>> position = {   {0, 0, 0, 0, 0, 0, 0},
                                       {0, 0, 0, 0, 0, 0, 0},
                                       {0, 0, 0, 0, 0, 0, 0},
                                       {0, 0, 0, 0, 0, 0, 0},
                                       {0, 0, 1, 0, 0, 0, 0},
                                       {0, 0, -1, 0, 0, 0, 1}};
    for (size_t index = 0; index < 42; index++) {
      REQUIRE(position[index / 7][index % 7] ==
              test.GetPieceAtLocation(index / 7, index % 7));
    }

    test.Reset();

    REQUIRE(test.GetIsXTurn());
    vector<vector<int>> cleared = {   {0, 0, 0, 0, 0, 0, 0},
                                       {0, 0, 0, 0, 0, 0, 0},
                                       {0, 0, 0, 0, 0, 0, 0},
                                       {0, 0, 0, 0, 0, 0, 0},
                                       {0, 0, 0, 0, 0, 0, 0},
                                       {0, 0, 0, 0, 0, 0, 0}};
    for (size_t index = 0; index < 42; index++) {
      REQUIRE(cleared[index / 7][index % 7] ==
              test.GetPieceAtLocation(index / 7, index % 7));
    }
  }

  SECTION("Game continues normally after reset") {
    test.DropPiece(1);
    test.DropPiece(2);
    test.DropPiece(1);
    test.DropPiece(2);
    test.DropPiece(1);
    test.DropPiece(2);
    test.DropPiece(1);
    REQUIRE(test.GetGameState() == BoardState::Xwins);
    test.Reset();

    REQUIRE(test.GetGameState() == BoardState::InProgress);
    test.DropPiece(6);
    test.DropPiece(2);
    test.DropPiece(2);

    REQUIRE(test.GetIsXTurn() == false);
    vector<vector<int>> position = {   {0, 0, 0, 0, 0, 0, 0},
                                       {0, 0, 0, 0, 0, 0, 0},
                                       {0, 0, 0, 0, 0, 0, 0},
                                       {0, 0, 0, 0, 0, 0, 0},
                                       {0, 0, 1, 0, 0, 0, 0},
                                       {0, 0, -1, 0, 0, 0, 1}};
    for (size_t index = 0; index < 42; index++) {
      REQUIRE(position[index / 7][index % 7] ==
              test.GetPieceAtLocation(index / 7, index % 7));
    }
  }
}

TEST_CASE("Test Generate Vector Features") {
  SECTION("Every piece is copied correctly into the vector") {
    vector<vector<int>> valid = {   {1, -1, 1, 1, -1, 1, 1},
                                    {-1, 1, -1, -1, 1, -1, -1},
                                    {1, -1, 1, 1, -1, 1, 1},
                                    {-1, 1, -1, -1, 1, -1, -1},
                                    {1, -1, 1, 1, -1, 1, 1},
                                    {-1, 1, -1, -1, 1, -1, -1}};
    GameBoard test(valid, true);
    std::vector<float> features = test.GenerateVectorFeatures();
    REQUIRE(features.size() == 42);
    for (size_t index = 0; index < 42; index++) {
      REQUIRE(features[index] == test.GetPieceAtLocation(index / 7, index % 7));
    }
  }
}

TEST_CASE("Calculate game state where X has won") {
  SECTION("Horizontal Win") {
    vector<vector<int>> valid = {   {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, -1, 0},
                                    {0, 0, 0, 0, 0, -1, 0},
                                    {1, 1, 1, 1, 0, -1, 0}};
    GameBoard test(valid, false);
    REQUIRE(test.GetGameState() == BoardState::Xwins);
  }

  SECTION("5 in a row Horizontal Win") {
    vector<vector<int>> valid = {   {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, -1, 0},
                                    {0, -1, 0, 0, 0, -1, 0},
                                    {1, 1, 1, 1, 1, -1, 0}};
    GameBoard test(valid, false);
    REQUIRE(test.GetGameState() == BoardState::Xwins);
  }

  SECTION("Vertical Win") {
    vector<vector<int>> valid = {   {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 1, 0, 0, 0},
                                    {0, 0, -1, 1, 0, 0, 0},
                                    {0, 0, -1, 1, 0, 0, 0},
                                    {0, 0, -1, 1, 0, 0, 0}};
    GameBoard test(valid, false);
    REQUIRE(test.GetGameState() == BoardState::Xwins);
  }

  SECTION("Up right diagonal win") {
    vector<vector<int>> valid = {   {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 1},
                                    {0, 0, 0, 0, 0, 1, 1},
                                    {0, 0, 0, 0, 1, 1, -1},
                                    {0, 0, -1, 1, -1, -1, -1}};
    GameBoard test(valid, false);
    REQUIRE(test.GetGameState() == BoardState::Xwins);
  }

  SECTION("Down right diagonal win") {
    vector<vector<int>> valid = {   {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {1, 0, 0, 0, 0, 0, 0},
                                    {1, 1, 0, 0, 0, 0, 0},
                                    {-1, -1, 1, 0, 0, 0, 0},
                                    {1, -1, -1, 1, -1, 0, 0}};
    GameBoard test(valid, false);
    REQUIRE(test.GetGameState() == BoardState::Xwins);
  }

  SECTION("5 in a row diagonal win") {
    vector<vector<int>> valid = {   {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 1, 0},
                                    {0, 0, 0, 0, 1, 1, 0},
                                    {0, 0, 0, 1, 1, -1, 0},
                                    {0, -1, 1, -1, -1, 1, 0},
                                    {-1, 1, -1, -1, 1, -1, 0}};
    GameBoard test(valid, false);
    REQUIRE(test.GetGameState() == BoardState::Xwins);
  }

  SECTION("Both diagonals win") {
    vector<vector<int>> valid = {   {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 1, 0, 1, 0, 0, 0},
                                    {0, -1, 1, -1, 0, 0, 0},
                                    {0, 1, 1, 1, 0, -1, 0},
                                    {1, -1, -1, -1, 1, -1, 0}};
    GameBoard test(valid, false);
    REQUIRE(test.GetGameState() == BoardState::Xwins);
  }
}

TEST_CASE("Calculate game state where O has won") {
  SECTION("Horizontal Win") {
    vector<vector<int>> valid = {   {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 1, 0, 0, 0, 0, 0},
                                    {0, 1, 0, 0, 0, 0, 0},
                                    {0, 1, -1, -1, -1, -1, 1}};
    GameBoard test(valid, true);
    REQUIRE(test.GetGameState() == BoardState::Owins);
  }

  SECTION("5 in a row Horizontal Win") {
    vector<vector<int>> valid = {   {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, -1, 0, 0, 0, 0},
                                    {0, 0, 1, 0, 0, 0, 0},
                                    {0, 0, 1, 1, 0, 0, 0},
                                    {-1, -1, -1, -1, -1, 1, 0},
                                    {1, -1, 1, 1, 1, -1, 0}};
    GameBoard test(valid, true);
    REQUIRE(test.GetGameState() == BoardState::Owins);
  }

  SECTION("Vertical Win") {
    vector<vector<int>> valid = {   {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, -1, 0, 0, 0},
                                    {0, 0, 0, -1, 0, 0, 0},
                                    {0, 0, 0, -1, -1, -1, 0},
                                    {1, 1, 1, -1, 1, 1, 1}};
    GameBoard test(valid, true);
    REQUIRE(test.GetGameState() == BoardState::Owins);
  }

  SECTION("Up right diagonal win") {
    vector<vector<int>> valid = {   {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, -1, 0},
                                    {0, 0, 0, 0, -1, 1, 0},
                                    {0, 0, 0, -1, 1, -1, 0},
                                    {0, 0, -1, 1, 1, 1, 0}};
    GameBoard test(valid, true);
    REQUIRE(test.GetGameState() == BoardState::Owins);
  }

  SECTION("Down right diagonal win") {
    vector<vector<int>> valid = {   {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, -1, 0, 0, 0, 0, 0},
                                    {0, 1, -1, 0, 0, 0, 0},
                                    {0, -1, -1, -1, 0, 1, 0},
                                    {0, 1, 1, 1, -1, 1, 0}};
    GameBoard test(valid, true);
    REQUIRE(test.GetGameState() == BoardState::Owins);
  }

  SECTION("Both diagonals win") {
    vector<vector<int>> valid = {   {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, -1, 0, -1, 0, 0},
                                    {0, 0, 1, -1, 1, 1, 0},
                                    {0, 0, -1, 1, -1, 1, 0},
                                    {1, -1, 1, 1, -1, -1, 0}};
    GameBoard test(valid, true);
    REQUIRE(test.GetGameState() == BoardState::Owins);
  }

  SECTION("5 in a row diagonal win") {
    vector<vector<int>> valid = {   {0, 0, 0, 0, 0, 0, 0},
                                    {0, -1, 0, 0, 0, 0, 0},
                                    {0, 1, -1, 0, 0, 0, 0},
                                    {0, -1, -1, -1, 0, 0, 0},
                                    {0, 1, 1, 1, -1, 0, 0},
                                    {1, 1, -1, 1, 1, -1, 0}};
    GameBoard test(valid, true);
    REQUIRE(test.GetGameState() == BoardState::Owins);
  }
}

TEST_CASE("Calculate game state where game is tied") {
  SECTION("Board is filled evaluates to tied game") {
    vector<vector<int>> valid = {   {1, -1, 1, 1, -1, 1, 1},
                                    {-1, 1, -1, -1, 1, -1, -1},
                                    {1, -1, 1, 1, -1, 1, 1},
                                    {-1, 1, -1, -1, 1, -1, -1},
                                    {1, -1, 1, 1, -1, 1, 1},
                                    {-1, 1, -1, -1, 1, -1, -1}};
    GameBoard test(valid, true);
    REQUIRE(test.GetGameState() == BoardState::Tie);
  }
}

TEST_CASE("Calculate game state where neither player has won") {
  SECTION("Empty board evaluates to neither player has won") {
    GameBoard test;
    REQUIRE(test.GetGameState() == BoardState::InProgress);
  }

  SECTION("Board in play evaluates to neither player has won") {
    vector<vector<int>> valid = {   {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {-1, -1, 0, -1, 0, -1, 0},
                                    {1, 1, 0, 1, 0, 1, 0}};
    GameBoard test(valid, true);
    REQUIRE(test.GetGameState() == BoardState::InProgress);
  }
}

TEST_CASE("Test calculate valid columns") {
  SECTION("Game ending should return empty vector") {
    vector<vector<int>> valid = {   {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, -1, 0, 0, 0, 0, 0},
                                    {0, 1, -1, 0, 0, 0, 0},
                                    {0, -1, -1, -1, 0, 1, 0},
                                    {0, 1, 1, 1, -1, 1, 0}};
    GameBoard test(valid, true);

    vector<size_t> expected_empty = {};
    REQUIRE(test.CalculateValidColumns() == expected_empty);
  }

  SECTION("Valid game returns non-filled columns") {
    vector<vector<int>> valid = {   {-1, 0, 0, 0, -1, 0, 0},
                                    {1, 0, 0, 0, 1, 1, 0},
                                    {-1, 0, 0, 0, -1, -1, 0},
                                    {1, 0, 0, 0, 1, 1, 0},
                                    {-1, 0, 0, 0, -1, -1, 0},
                                    {1, 0, 0, 0, 1, 1, -1}};
    GameBoard test(valid, true);

    vector<size_t> expected_empty = {3, 2, 5, 1, 6};
    REQUIRE(test.CalculateValidColumns() == expected_empty);
  }
}

TEST_CASE("Test drop piece") {
  SECTION("Dropping piece out of bounds throws exception") {
    vector<vector<int>> valid = {   {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0}};
    GameBoard test(valid, true);
    REQUIRE_THROWS_AS(test.DropPiece(7), std::out_of_range);
  }

  SECTION("Dropping piece when game is over does nothing") {
    vector<vector<int>> valid = {   {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, -1, 0, 0, 0},
                                    {0, 0, 0, -1, 0, 0, 0},
                                    {0, 0, 0, -1, -1, -1, 0},
                                    {1, 1, 1, -1, 1, 1, 1}};
    GameBoard test(valid, true);

    // Drop a piece and check that nothing has changed
    REQUIRE(test.DropPiece(0) == false);
    REQUIRE(test.GetIsXTurn() == true);
    REQUIRE(test.GetPieceAtLocation(4, 0) == test.kEmpty);
    REQUIRE(test.GetGameState() == BoardState::Owins);
  }

  SECTION("Dropping piece in filled column does nothing") {
    vector<vector<int>> valid = {   {-1, 0, 0, 0, -1, 0, 0},
                                    {1, 0, 0, 0, 1, 1, 0},
                                    {-1, 0, 0, 0, -1, -1, 0},
                                    {1, 0, 0, 0, 1, 1, 0},
                                    {-1, 0, 0, 0, -1, -1, 0},
                                    {1, 0, 0, 0, 1, 1, -1}};
    GameBoard test(valid, true);

    REQUIRE(test.DropPiece(4) == false);
    REQUIRE(test.GetIsXTurn() == true);
    REQUIRE(test.GetPieceAtLocation(0, 4) == test.kOPiece);
    REQUIRE(test.GetGameState() == BoardState::InProgress);
  }

  SECTION("Dropping piece in valid columns work properly") {
    vector<vector<int>> valid = {   {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0},
                                    {-1, -1, 0, -1, 0, -1, 0},
                                    {1, 1, 0, 1, 0, 1, 0}};
    GameBoard test(valid, true);
    REQUIRE(test.DropPiece(6));

    REQUIRE(test.GetIsXTurn() == false);
    REQUIRE(test.GetPieceAtLocation(5, 6) == test.kXPiece);
    REQUIRE(test.GetGameState() == BoardState::InProgress);

    REQUIRE(test.DropPiece(1));
    REQUIRE(test.GetIsXTurn() == true);
    REQUIRE(test.GetPieceAtLocation(3, 1) == test.kOPiece);
    REQUIRE(test.GetGameState() == BoardState::InProgress);
  }
}


