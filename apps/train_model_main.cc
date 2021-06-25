#include <iostream>

#include "tiny_dnn/tiny_dnn.h"
#include <core/data_parser.h>

using namespace tiny_dnn;
using namespace tiny_dnn::activation;
using namespace tiny_dnn::layers;

int main(int argc, char *argv[]) {
  // Tutorial: https://gist.github.com/marty1885/dd648a1806348bf4cd2c2fd0feafae36
  std::cout << "Hi!" << std::endl;

  connect_four::DataParser parser;
  parser.YieldTestDataNumericCSV("data/c4_game_database.csv", 1,
                          1640);
  parser.YieldTestDataStringCSV("data/connect-4.data", 1,
                           1557);
  std::vector<tiny_dnn::vec_t> testIn = parser.GetTestFeatures();
  std::vector<tiny_dnn::label_t> testOut = parser.GetTestLabels();

  network<sequential> net;
  net << fully_connected_layer(42,128) << relu()
      << fully_connected_layer(128,64) << relu()
      << fully_connected_layer(64,3) << softmax();

  adam optimizer;

  // Train and validate the model (375000 training examples)
  // Number of loops through the entire training dataset
  size_t epochs = 4;

  for (size_t epoch = 0; epoch < epochs; epoch++) {
    std::cout << "Epoch: " << epoch << std::endl;
    std::cout << "Training..." << std::endl;

    // Use 24 to 375 for c4_game_database, first 1640 positions as test
    // For connect-4.data, there are 66000 training positions
    for (size_t batch = 0; batch < 66; batch++) {
      if (batch % 10 == 0) {
        std::cout << "Step: " << batch << std::endl;
      }

      parser.Clear();
      parser.YieldTrainingDataNumericCSV("data/c4_game_database.csv",
                                   1640 + (24 + batch) * 1000,
                                   1000);
      parser.YieldTrainingDataStringCSV("data/connect-4.data",
                                   1557 + batch * 1000,
                            1000);
      std::vector<tiny_dnn::vec_t> trainIn = parser.GetTrainFeatures();
      std::vector<tiny_dnn::label_t> trainOut = parser.GetTrainLabels();
      size_t batch_size = trainIn.size();
      net.train<cross_entropy>(optimizer, trainIn, trainOut, batch_size, 1);
    }

    std::cout << "Testing..." << std::endl;
    result res = net.test(testIn, testOut);
    std::cout << res.num_success << "/" << res.num_total << std::endl;
  }

  net.save("net_2");
  return 0;
}
