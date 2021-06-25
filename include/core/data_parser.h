#pragma once

#include <iostream>
#include <vector>
#include "tiny_dnn/tiny_dnn.h"

namespace connect_four {

using std::istream;
using std::string;

/**
 * Reads data from a csv file and turns it into vec_t format
 * to train and validate a model
 */
class DataParser {
 public:
  // Constants for data parsing
  const size_t kWidth = 7;
  const size_t kHeight = 6;
  const int kXWinsCategory = 2;
  const int kTieCategory = 1;
  const int kOWinsCategory = 0;

  DataParser() = default;

  /**
   * Processes CSVs where pieces are represented as 1, -1, or 0 for blank.
   * The result is indicated as -1 for a yellow win, 1 for a red win, 0 for tie
   *
   * Assumes the last column is the label. Start is the row to start from,
   * and this method will yield a specified amount of rows as training examples.
   *
   * Start is one-indexed, so if start is 1 then the first non-header row
   * will be processed as an example.
   *
   * Updates train_features and train_labels
   */
  void YieldTrainingDataNumericCSV(const std::string& csv_path, size_t start,
                            size_t number_examples);

  /**
   * Processes CSVs where pieces are represented as 1, -1, or 0 for blank.
   * The result is indicated as -1 for a yellow win, 1 for a red win, 0 for tie
   *
   * Assumes the last column is the label. Start is the row to start from,
   * and this method will yield a specified amount of rows as test examples.
   *
   * Start is one-indexed, so if start is 1 then the first non-header row
   * will be processed as an example.
   *
   * Updates test_features and test_labels
   */
  void YieldTestDataNumericCSV(const std::string& csv_path, size_t start,
                            size_t number_examples);

  /**
   * Processes CSVs where pieces are represented as x, o, or b for blank.
   * The result is indicated as "win", "loss", or "draw"
   *
   * Assumes the last column is the label. Start is the row to start from,
   * and this method will yield a specified amount of rows as training examples.
   *
   * Start is one-indexed, so if start is 1 then the first non-header row
   * will be processed as an example.
   *
   * Updates train_features and train_labels
   */
  void YieldTrainingDataStringCSV(const std::string& csv_path, size_t start,
                            size_t number_examples);

  /**
   * Processes CSVs where pieces are represented as x, o, or b for blank.
   * The result is indicated as "win", "loss", or "draw"
   *
   * Assumes the last column is the label. Start is the row to start from,
   * and this method will yield a specified amount of rows as test examples.
   *
   * Start is one-indexed, so if start is 1 then the first non-header row
   * will be processed as an example.
   *
   * Updates test_features and test_labels
   */
  void YieldTestDataStringCSV(const std::string& csv_path, size_t start,
                        size_t number_examples);

  /**
   * Since all the data can't be loaded in-memory, this will reset
   * all the stored vectors.
   */
  void Clear();

  // Getters
  const std::vector<tiny_dnn::vec_t>& GetTrainFeatures() const;
  const std::vector<tiny_dnn::label_t>& GetTrainLabels() const;
  const std::vector<tiny_dnn::vec_t>& GetTestFeatures() const;
  const std::vector<tiny_dnn::label_t>& GetTestLabels() const;

 private:
  std::vector<tiny_dnn::vec_t> train_features;
  std::vector<tiny_dnn::label_t> train_labels;
  std::vector<tiny_dnn::vec_t> test_features;
  std::vector<tiny_dnn::label_t> test_labels;

  /**
   * A helper function to split strings by character.
   * @param line The string to be split.
   * @param delim The delimiter to split by.
   * @return A vector of strings split by the delimiting character.
   */
  std::vector<string> Split(const string &line, char delim) const;
};

} // namespace connect_four