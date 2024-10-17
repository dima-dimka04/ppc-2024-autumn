// Copyright 2024 Nesterov Alexander
#include "seq/drozhdinov_d_sum_cols_matrix/include/ops_seq.hpp"

#include <thread>

using namespace std::chrono_literals;

int linearizeCoordinates(int x, int y, int xSize) { return y * xSize + x; }

std::vector<int> calculateMatrixSumSequentially(const std::vector<int>& matrix, int xSize, int ySize, int fromX,
                                                int toX) {
  std::vector<int> result;
  for (int x = fromX; x < toX; x++) {
    int columnSum = 0;
    for (int y = 0; y < ySize; y++) {
      int linearizedCoordinate = linearizeCoordinates(x, y, xSize);
      columnSum += matrix[linearizedCoordinate];
    }
    result.push_back(columnSum);
  }
  return result;
}

std::vector<int> calculateMatrixSumSequentially(const std::vector<int>& matrix, int xSize, int ySize) {
  return calculateMatrixSumSequentially(matrix, xSize, ySize, 0, xSize);
}

bool drozhdinov_d_sum_cols_matrix_seq::TestTaskSequential::pre_processing() {
  internal_order_test();
  // Init value for input and output
  input_ = std::vector<int>(taskData->inputs_count[0]);
  auto* ptr = reinterpret_cast<int*>(taskData->inputs[0]);
  for (size_t i = 0; i < taskData->inputs_count[0]; i++) {
    input_[i] = ptr[i];
  }
  cols = taskData->inputs_count[1];
  rows = taskData->inputs_count[2];
  res = std::vector<int>(cols, 0);
  return true;
}

bool drozhdinov_d_sum_cols_matrix_seq::TestTaskSequential::validation() {
  internal_order_test();
  // Check count elements of output
  return taskData->inputs_count[1] == taskData->outputs_count[0];
}

bool drozhdinov_d_sum_cols_matrix_seq::TestTaskSequential::run() {
  internal_order_test();
  res = calculateMatrixSumSequentially(input_, cols, rows);
  return true;
}

bool drozhdinov_d_sum_cols_matrix_seq::TestTaskSequential::post_processing() {
  internal_order_test();
  for (size_t i = 0; i < cols; i++) {
    reinterpret_cast<int*>(taskData->outputs[0])[i] = res[i];
  }
  return true;
}