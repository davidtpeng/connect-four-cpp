#include <core/data_parser.h>

namespace connect_four {

void DataParser::YieldTrainingDataNumericCSV(const std::string& csv_path,
                                             size_t start,
                                             size_t number_examples) {
  string line;
  std::ifstream csv;
  csv.open(csv_path);

  // Skip to get to the starting line
  for (size_t i = 0; i < start; i++) {
    getline(csv, line);
  }

  if (!csv.good()) {
    throw std::invalid_argument("File stream is not good");
  }

  // Process a number of examples
  for (size_t i = 0; i < number_examples; i++) {
    if (csv.eof()) {
      break;
    }

    getline(csv, line);
    std::vector<string> splitted = Split(line, ',');

    // Skip empty lines
    if (splitted.size() == 0) {
      continue;
    }

    // Features and label
    tiny_dnn::vec_t features;
    tiny_dnn::label_t label;

    for (size_t index = 0; index < splitted.size(); index++) {
      // Last column is label, all else are features
      if (index != splitted.size() - 1) {
        features.push_back(stof(splitted[index]));
      } else {
        // Since labels are stored as -1, 1, or 0, add 1 to
        // scale to 0 to 2 range for categorization
        label = stof(splitted[index]) + 1;
      }
    }

    // Check shapes
    if (features.size() != kWidth * kHeight) {
      continue;
    }

    // Save to training
    train_labels.push_back(label);
    train_features.push_back(features);
  }
}

void DataParser::YieldTestDataNumericCSV(const std::string& csv_path,
                                         size_t start,
                                         size_t number_examples) {
  string line;
  std::ifstream csv;
  csv.open(csv_path);

  // Skip to get to the starting line
  for (size_t i = 0; i < start; i++) {
    getline(csv, line);
  }

  if (!csv.good()) {
    throw std::invalid_argument("File stream is not good");
  }

  // Process a number of examples
  for (size_t i = 0; i < number_examples; i++) {
    if (csv.eof()) {
      break;
    }

    getline(csv, line);
    std::vector<string> splitted = Split(line, ',');

    // Skip empty lines
    if (splitted.size() == 0) {
      continue;
    }

    // Features and label
    tiny_dnn::vec_t features;
    tiny_dnn::label_t label;

    for (size_t index = 0; index < splitted.size(); index++) {
      // Last column is label, all else are features
      if (index != splitted.size() - 1) {
        features.push_back(stof(splitted[index]));
      } else {
        // Since labels are stored as -1, 1, or 0, add 1 to
        // scale to 0 to 2 range for categorization
        label = stof(splitted[index]) + 1;
      }
    }

    // Check shapes
    if (features.size() != kWidth * kHeight) {
      continue;
    }

    // Save to testing
    test_labels.push_back(label);
    test_features.push_back(features);
  }
}

void DataParser::YieldTrainingDataStringCSV(const std::string& csv_path,
                                            size_t start,
                                            size_t number_examples) {
  string line;
  std::ifstream csv;
  csv.open(csv_path);

  // Skip to get to the starting line
  for (size_t i = 1; i < start; i++) {
    getline(csv, line);
  }

  if (!csv.good()) {
    throw std::invalid_argument("File stream is not good");
  }

  // Process a number of examples
  for (size_t i = 0; i < number_examples; i++) {
    if (csv.eof()) {
      break;
    }

    getline(csv, line);
    std::vector<string> splitted = Split(line, ',');

    // Skip empty lines
    if (splitted.size() == 0) {
      continue;
    }

    // Features and label
    tiny_dnn::vec_t features;
    tiny_dnn::label_t label;

    // The order of the pieces in by column, so store here first and then
    // convert to vec_t
    std::vector<std::vector<int>> integer_features;
    integer_features.resize(kHeight, std::vector<int>(kWidth, 0));

    for (size_t index = 0; index < splitted.size(); index++) {
      // Last column is label, all else are features
      if (index != splitted.size() - 1) {
        if (splitted[index] == "b") {
          integer_features[kHeight - 1 - index % kHeight][index / kHeight] = 0;
        } else if (splitted[index] == "x") {
          integer_features[kHeight - 1 - index % kHeight][index / kHeight] = 1;
        } else if (splitted[index] == "o") {
          integer_features[kHeight - 1 - index % kHeight][index / kHeight] = -1;
        }
      } else {
        // Label is win loss or draw
        if (splitted[index] == "win") {
          label = kXWinsCategory;
        } else if (splitted[index] == "draw") {
          label = kTieCategory;
        } else if (splitted[index] == "loss") {
          label = kOWinsCategory;
        }
      }
    }

    for (size_t index = 0; index < kWidth * kHeight; index++) {
      features.push_back(integer_features[index / kWidth][index % kWidth]);
    }

    // Save to training
    train_labels.push_back(label);
    train_features.push_back(features);
  }
}

void DataParser::YieldTestDataStringCSV(const std::string& csv_path,
                                        size_t start,
                                        size_t number_examples) {
  string line;
  std::ifstream csv;
  csv.open(csv_path);

  // Skip to get to the starting line
  for (size_t i = 1; i < start; i++) {
    getline(csv, line);
  }

  if (!csv.good()) {
    throw std::invalid_argument("File stream is not good");
  }

  // Process a number of examples
  for (size_t i = 0; i < number_examples; i++) {
    if (csv.eof()) {
      break;
    }

    getline(csv, line);
    std::vector<string> splitted = Split(line, ',');

    // Skip empty lines
    if (splitted.size() == 0) {
      continue;
    }

    // Features and label
    tiny_dnn::vec_t features;
    tiny_dnn::label_t label;

    // The order of the pieces in by column, so store here first and then
    // convert to vec_t
    std::vector<std::vector<int>> integer_features;
    integer_features.resize(kHeight, std::vector<int>(kWidth, 0));

    for (size_t index = 0; index < splitted.size(); index++) {
      // Last column is label, all else are features
      if (index != splitted.size() - 1) {
        if (splitted[index] == "b") {
          integer_features[kHeight - 1 - index % kHeight][index / kHeight] = 0;
        } else if (splitted[index] == "x") {
          integer_features[kHeight - 1 - index % kHeight][index / kHeight] = 1;
        } else if (splitted[index] == "o") {
          integer_features[kHeight - 1 - index % kHeight][index / kHeight] = -1;
        }
      } else {
        // Label is win loss or draw
        if (splitted[index] == "win") {
          label = kXWinsCategory;
        } else if (splitted[index] == "draw") {
          label = kTieCategory;
        } else if (splitted[index] == "loss") {
          label = kOWinsCategory;
        }
      }
    }

    for (size_t index = 0; index < kWidth * kHeight; index++) {
      features.push_back(integer_features[index / kWidth][index % kWidth]);
    }

    // Save to training
    test_labels.push_back(label);
    test_features.push_back(features);
  }
}

void DataParser::Clear() {
  train_features.clear();
  train_labels.clear();
  test_features.clear();
  test_labels.clear();
}

const std::vector<tiny_dnn::vec_t>& DataParser::GetTrainFeatures() const {
  return train_features;
}

const std::vector<tiny_dnn::label_t>& DataParser::GetTrainLabels() const {
  return train_labels;
}

const std::vector<tiny_dnn::vec_t>& DataParser::GetTestFeatures() const {
  return test_features;
}

const std::vector<tiny_dnn::label_t>& DataParser::GetTestLabels() const {
  return test_labels;
}

std::vector<string> DataParser::Split(const string& line, char delim) const {
  std::stringstream string_stream(line);
  std::vector<string> elems;
  string item;
  while (std::getline(string_stream, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

} // namespace connect_four