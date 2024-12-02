// Copyright 2024 Flamingos Technologies Inc. All Rights Reserved.
//
// PROPRIETARY AND CONFIDENTIAL
//
// This software and its documentation are the confidential and proprietary
// information of Flamingos Technologies Inc. ("Confidential Information").
// You shall not disclose such Confidential Information and shall use it
// only in accordance with the terms of the license agreement you entered
// into with Flamingos Technologies Inc. Unauthorized copying, redistribution,
// or use of this software in source or binary forms is strictly prohibited.

#include <cstdint>
#include <vector>

#include "mediapipe/calculators/tensorflow/matrix_to_tensor_calculator_options.pb.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/calculator_runner.h"
#include "mediapipe/framework/formats/matrix.h"
#include "mediapipe/framework/port/gtest.h"
#include "mediapipe/framework/port/status_matchers.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/framework/tensor_shape.h"
#include "tensorflow/core/framework/types.h"

namespace mediapipe {
namespace {

constexpr char kTransposeOptionsString[] =
    "[mediapipe.MatrixToTensorCalculatorOptions.ext]: {"
    "transpose: True}";
constexpr char kAddDimensionOptionsString[] =
    "[mediapipe.MatrixToTensorCalculatorOptions.ext]: {"
    "add_trailing_dimension: True}";

}  // namespace

namespace tf = tensorflow;
using RandomEngine = std::mt19937_64;
const uint32_t kSeed = 1234;
const int kNumSizes = 8;
const int sizes[kNumSizes][2] = {{1, 1}, {12, 1}, {1, 9},   {2, 2},
                                 {5, 3}, {7, 13}, {16, 32}, {101, 2}};

class MatrixToTensorCalculatorTest : public ::testing::Test {
 protected:
  // Adds a packet with a matrix filled with random values in [0,1].
  void AddRandomMatrix(int num_rows, int num_columns, uint32_t seed) {
    RandomEngine random(kSeed);
    std::uniform_real_distribution<> uniform_dist(0, 1.0);
    auto matrix = ::absl::make_unique<Matrix>();
    matrix->resize(num_rows, num_columns);
    for (int y = 0; y < num_rows; ++y) {
      for (int x = 0; x < num_columns; ++x) {
        (*matrix)(y, x) = uniform_dist(random);
      }
    }
    runner_->MutableInputs()->Index(0).packets.push_back(
        Adopt(matrix.release()).At(Timestamp(0)));
  }

  std::unique_ptr<CalculatorRunner> runner_;
};

TEST_F(MatrixToTensorCalculatorTest, RandomMatrix) {
  for (int size_index = 0; size_index < kNumSizes; ++size_index) {
    const int num_rows = sizes[size_index][0];
    const int num_columns = sizes[size_index][1];

    // Run the calculator and verify that one output is generated.
    runner_ = ::absl::make_unique<CalculatorRunner>("MatrixToTensorCalculator",
                                                    "", 1, 1, 0);
    AddRandomMatrix(num_rows, num_columns, kSeed);
    MP_ASSERT_OK(runner_->Run());
    const std::vector<Packet>& output_packets =
        runner_->Outputs().Index(0).packets;
    ASSERT_EQ(1, output_packets.size());

    // Verify that the packet contains a 2D float tensor.
    const tf::Tensor& tensor = output_packets[0].Get<tf::Tensor>();
    ASSERT_EQ(2, tensor.dims());
    ASSERT_EQ(tf::DT_FLOAT, tensor.dtype());

    // Verify that the data is correct.
    RandomEngine random(kSeed);
    std::uniform_real_distribution<> uniform_dist(0, 1.0);
    const auto matrix = tensor.matrix<float>();
    for (int y = 0; y < num_rows; ++y) {
      for (int x = 0; x < num_columns; ++x) {
        const float expected = uniform_dist(random);
        ASSERT_EQ(expected, matrix(y, x));
      }
    }
  }
}

TEST_F(MatrixToTensorCalculatorTest, RandomMatrixTranspose) {
  for (int size_index = 0; size_index < kNumSizes; ++size_index) {
    const int num_rows = sizes[size_index][0];
    const int num_columns = sizes[size_index][1];

    // Run the calculator and verify that one output is generated.
    runner_ = ::absl::make_unique<CalculatorRunner>(
        "MatrixToTensorCalculator", kTransposeOptionsString, 1, 1, 0);
    AddRandomMatrix(num_rows, num_columns, kSeed);
    MP_ASSERT_OK(runner_->Run());
    const std::vector<Packet>& output_packets =
        runner_->Outputs().Index(0).packets;
    ASSERT_EQ(1, output_packets.size());

    // Verify that the packet contains a 2D float tensor.
    const tf::Tensor& tensor = output_packets[0].Get<tf::Tensor>();
    ASSERT_EQ(2, tensor.dims());
    ASSERT_EQ(tf::DT_FLOAT, tensor.dtype());

    // Verify that the data is correct.
    RandomEngine random(kSeed);
    std::uniform_real_distribution<> uniform_dist(0, 1.0);
    const auto matrix = tensor.matrix<float>();
    for (int y = 0; y < num_rows; ++y) {
      for (int x = 0; x < num_columns; ++x) {
        const float expected = uniform_dist(random);
        ASSERT_EQ(expected, matrix(x, y));
      }
    }
  }
}

TEST_F(MatrixToTensorCalculatorTest, RandomMatrixAddDimension) {
  for (int size_index = 0; size_index < kNumSizes; ++size_index) {
    const int num_rows = sizes[size_index][0];
    const int num_columns = sizes[size_index][1];

    // Run the calculator and verify that one output is generated.
    runner_ = ::absl::make_unique<CalculatorRunner>(
        "MatrixToTensorCalculator", kAddDimensionOptionsString, 1, 1, 0);
    AddRandomMatrix(num_rows, num_columns, kSeed);
    MP_ASSERT_OK(runner_->Run());
    const std::vector<Packet>& output_packets =
        runner_->Outputs().Index(0).packets;
    ASSERT_EQ(1, output_packets.size());

    // Verify that the packet contains a 3D float tensor.
    const tf::Tensor& tensor = output_packets[0].Get<tf::Tensor>();
    ASSERT_EQ(3, tensor.dims());
    ASSERT_EQ(tf::DT_FLOAT, tensor.dtype());

    // Verify that the data is correct.
    RandomEngine random(kSeed);
    std::uniform_real_distribution<> uniform_dist(0, 1.0);
    // const auto matrix = tensor.matrix<float>();
    const float* tensor_data = tensor.flat<float>().data();
    for (int y = 0; y < num_rows; ++y) {
      for (int x = 0; x < num_columns; ++x) {
        const float expected = uniform_dist(random);
        ASSERT_EQ(expected, tensor_data[y * num_columns + x]);
      }
    }
  }
}

}  // namespace mediapipe
