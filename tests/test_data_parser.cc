#include <catch2/catch.hpp>

#include <core/data_parser.h>

using connect_four::DataParser;

TEST_CASE("Read Example Numeric CSV") {
  DataParser test;

  SECTION("Read both rows as training examples") {
    test.YieldTrainingDataNumericCSV("data/c4_short_database.csv", 1,
                              2);
    std::vector<tiny_dnn::vec_t> trainIn = test.GetTrainFeatures();
    std::vector<tiny_dnn::label_t> trainOut = test.GetTrainLabels();

    std::vector<tiny_dnn::label_t> expected_labels({0, 2});
    std::vector<tiny_dnn::vec_t> expected_features({{1,1,1,-1,-1,1,0,-1,-1,-1,1,
                                                     -1,-1,0,-1,1,1,-1,1,1,0,1,
                                                     -1,1,-1,-1,-1,-1,1,1,-1,-1,
                                                     1,1,1,-1,1,-1,1,-1,1,-1},
                                                    {0,0,1,1,1,1,0,0,0,1,-1,-1,
                                                     -1,0,0,0,1,1,1,-1,0,0,0,-1,
                                                     -1,1,1,-1,1,-1,-1,1,-1,-1,1
                                                     ,1,-1,-1,-1,1,1,-1}});
    REQUIRE(trainIn == expected_features);
    REQUIRE(trainOut == expected_labels);
  }

  SECTION("Read one row as a training example") {
    test.YieldTestDataNumericCSV("data/c4_short_database.csv", 1,
                              1);
    test.YieldTrainingDataNumericCSV("data/c4_short_database.csv", 2,
                          1);
    std::vector<tiny_dnn::vec_t> trainIn = test.GetTrainFeatures();
    std::vector<tiny_dnn::label_t> trainOut = test.GetTrainLabels();
    std::vector<tiny_dnn::vec_t> testIn = test.GetTestFeatures();
    std::vector<tiny_dnn::label_t> testOut = test.GetTestLabels();

    std::vector<tiny_dnn::label_t> expected_test_labels({0});
    std::vector<tiny_dnn::vec_t> expected_test_features({{1,1,1,-1,-1,1,0,-1,-1,-1,1,
                                                     -1,-1,0,-1,1,1,-1,1,1,0,1,
                                                     -1,1,-1,-1,-1,-1,1,1,-1,-1,
                                                     1,1,1,-1,1,-1,1,-1,1,-1}});
    std::vector<tiny_dnn::label_t> expected_train_labels({2});
    std::vector<tiny_dnn::vec_t> expected_train_features({{0,0,1,1,1,1,0,0,0,1,-1,-1,
                                                              -1,0,0,0,1,1,1,-1,0,0,0,-1,
                                                              -1,1,1,-1,1,-1,-1,1,-1,-1,1
                                                              ,1,-1,-1,-1,1,1,-1}});
    REQUIRE(trainIn == expected_train_features);
    REQUIRE(trainOut == expected_train_labels);
    REQUIRE(testIn == expected_test_features);
    REQUIRE(testOut == expected_test_labels);
  }

  SECTION("Read Dataset And Clear") {
    test.YieldTestDataNumericCSV("data/c4_game_database.csv", 1,
                          1640);
    test.YieldTrainingDataNumericCSV("data/c4_game_database.csv",
                                     1640,1000);
    std::vector<tiny_dnn::vec_t> trainIn = test.GetTrainFeatures();
    std::vector<tiny_dnn::label_t> trainOut = test.GetTrainLabels();
    std::vector<tiny_dnn::vec_t> testIn = test.GetTestFeatures();
    std::vector<tiny_dnn::label_t> testOut = test.GetTestLabels();

    REQUIRE(trainIn.size() == 1000);
    REQUIRE(trainOut.size() == 1000);
    REQUIRE(testIn.size() == 1640);
    REQUIRE(testOut.size() == 1640);

    // Also test clearing
    test.Clear();
    trainIn = test.GetTrainFeatures();
    trainOut = test.GetTrainLabels();
    testIn = test.GetTestFeatures();
    testOut = test.GetTestLabels();
    REQUIRE(trainIn.size() == 0);
    REQUIRE(trainOut.size() == 0);
    REQUIRE(testIn.size() == 0);
    REQUIRE(testOut.size() == 0);
  }
}

TEST_CASE("Read Example CSV2") {
  DataParser test;

  SECTION("Read two rows as training examples") {
    test.YieldTrainingDataStringCSV("data/connect-4.data", 6,
                              2);
    std::vector<tiny_dnn::vec_t> trainIn = test.GetTrainFeatures();
    std::vector<tiny_dnn::label_t> trainOut = test.GetTrainLabels();

    std::vector<tiny_dnn::label_t> expected_labels({2, 1});
    /*
     * {0, 0, 0, -1, 0, 0, 0},
       {0, 0, 0, 1, 0, 0, 0},
       {0, 0, 0, -1, 0, 0, 0},
       {0, 0, 0, 1, 0, 0, 0},
       {0, 0, 0, -1, 0, 0, 0},
       {0, 0, 1, 1, 0, 0, -1}  win

     * {0, 0, 0, -1, 0, 0, 0},
       {0, 0, 0, 1, 0, 0, 0},
       {0, 0, 0, -1, 0, 0, 0},
       {0, 0, 0, 1, 0, 0, 0},
       {0, 0, 0, -1, 0, 0, 0},
       {0, 1, -1, 1, 0, 0, 0} draw
     */
    std::vector<tiny_dnn::vec_t> expected_features({{0, 0, 0, -1, 0, 0, 0,
                                                     0, 0, 0, 1, 0, 0, 0,
                                                     0, 0, 0, -1, 0, 0, 0,
                                                     0, 0, 0, 1, 0, 0, 0,
                                                     0, 0, 0, -1, 0, 0, 0,
                                                     0, 0, 1, 1, 0, 0, -1},
                                                    {0, 0, 0, -1, 0, 0, 0,
                                                     0, 0, 0, 1, 0, 0, 0,
                                                     0, 0, 0, -1, 0, 0, 0,
                                                     0, 0, 0, 1, 0, 0, 0,
                                                     0, 0, 0, -1, 0, 0, 0,
                                                     0, 1, -1, 1, 0, 0, 0}});
    REQUIRE(trainIn == expected_features);
    REQUIRE(trainOut == expected_labels);
  }

  SECTION("Read one row as a training example") {
    test.YieldTestDataStringCSV("data/connect-4.data", 6,
                               1);
    test.YieldTrainingDataStringCSV("data/connect-4.data", 7,
                           1);

    std::vector<tiny_dnn::vec_t> trainIn = test.GetTrainFeatures();
    std::vector<tiny_dnn::label_t> trainOut = test.GetTrainLabels();
    std::vector<tiny_dnn::vec_t> testIn = test.GetTestFeatures();
    std::vector<tiny_dnn::label_t> testOut = test.GetTestLabels();

    std::vector<tiny_dnn::label_t> expected_test_labels({2});
    std::vector<tiny_dnn::vec_t> expected_test_features({{0, 0, 0, -1, 0, 0, 0,
                                                          0, 0, 0, 1, 0, 0, 0,
                                                          0, 0, 0, -1, 0, 0, 0,
                                                          0, 0, 0, 1, 0, 0, 0,
                                                          0, 0, 0, -1, 0, 0, 0,
                                                          0, 0, 1, 1, 0, 0, -1}});
    std::vector<tiny_dnn::label_t> expected_train_labels({1});
    std::vector<tiny_dnn::vec_t> expected_train_features({{0, 0, 0, -1, 0, 0, 0,
                                                              0, 0, 0, 1, 0, 0, 0,
                                                              0, 0, 0, -1, 0, 0, 0,
                                                              0, 0, 0, 1, 0, 0, 0,
                                                              0, 0, 0, -1, 0, 0, 0,
                                                              0, 1, -1, 1, 0, 0, 0}});
    REQUIRE(trainIn == expected_train_features);
    REQUIRE(trainOut == expected_train_labels);
    REQUIRE(testIn == expected_test_features);
    REQUIRE(testOut == expected_test_labels);
  }

  SECTION("Read Dataset And Clear") {
    test.YieldTestDataStringCSV("data/connect-4.data", 1,
                          1557);
    test.YieldTrainingDataStringCSV("data/connect-4.data", 1557,
                              1000);
    std::vector<tiny_dnn::vec_t> trainIn = test.GetTrainFeatures();
    std::vector<tiny_dnn::label_t> trainOut = test.GetTrainLabels();
    std::vector<tiny_dnn::vec_t> testIn = test.GetTestFeatures();
    std::vector<tiny_dnn::label_t> testOut = test.GetTestLabels();

    REQUIRE(trainIn.size() == 1000);
    REQUIRE(trainOut.size() == 1000);
    REQUIRE(testIn.size() == 1557);
    REQUIRE(testOut.size() == 1557);

    // Also test clearing
    test.Clear();
    trainIn = test.GetTrainFeatures();
    trainOut = test.GetTrainLabels();
    testIn = test.GetTestFeatures();
    testOut = test.GetTestLabels();
    REQUIRE(trainIn.size() == 0);
    REQUIRE(trainOut.size() == 0);
    REQUIRE(testIn.size() == 0);
    REQUIRE(testOut.size() == 0);
  }
}