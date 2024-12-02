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
//
// Verifies the correctness of parallel execution.
// $ bazel build -c opt \
//   mediapipe/framework/calculator_parallel_execution_test \
//   --runs_per_test=100
//
// TODO: Add more tests to verify the correctness of parallel execution.

#include <memory>
#include <random>
#include <string>
#include <vector>

#include "absl/memory/memory.h"
#include "absl/synchronization/mutex.h"
#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/gmock.h"
#include "mediapipe/framework/port/gtest.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/port/status_matchers.h"

namespace mediapipe {

namespace {

using RandomEngine = std::mt19937_64;

inline void BusySleep(absl::Duration duration) {
  absl::Time start_time = absl::Now();
  while (absl::Now() - start_time < duration) {
  }
}

class SlowPlusOneCalculator : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc) {
    cc->Inputs().Index(0).Set<int>();
    cc->Outputs().Index(0).Set<int>();
    return absl::OkStatus();
  }

  absl::Status Open(CalculatorContext* cc) override {
    cc->SetOffset(mediapipe::TimestampDiff(0));
    return absl::OkStatus();
  }

  absl::Status Process(CalculatorContext* cc) override {
    if (cc->InputTimestamp().Value() % 4 == 0) {
      return absl::OkStatus();
    }

    RandomEngine random(testing::UnitTest::GetInstance()->random_seed());
    std::uniform_int_distribution<> uniform_dist(0, 10);
    BusySleep(absl::Milliseconds(90 + uniform_dist(random)));
    cc->Outputs().Index(0).Add(new int(cc->Inputs().Index(0).Get<int>() + 1),
                               cc->InputTimestamp());
    return absl::OkStatus();
  }
};

REGISTER_CALCULATOR(SlowPlusOneCalculator);

class ParallelExecutionTest : public testing::Test {
 public:
  void AddThreadSafeVectorSink(const Packet& packet) {
    absl::WriterMutexLock lock(&output_packets_mutex_);
    output_packets_.push_back(packet);
  }

 protected:
  std::vector<Packet> output_packets_ ABSL_GUARDED_BY(output_packets_mutex_);
  absl::Mutex output_packets_mutex_;
};

TEST_F(ParallelExecutionTest, SlowPlusOneCalculatorsTest) {
  CalculatorGraphConfig graph_config =
      mediapipe::ParseTextProtoOrDie<CalculatorGraphConfig>(R"pb(
        input_stream: "input"
        node {
          calculator: "SlowPlusOneCalculator"
          input_stream: "input"
          output_stream: "first_calculator_output"
          max_in_flight: 5
        }
        node {
          calculator: "SlowPlusOneCalculator"
          input_stream: "first_calculator_output"
          output_stream: "output"
          max_in_flight: 5
        }
        node {
          calculator: "CallbackCalculator"
          input_stream: "output"
          input_side_packet: "CALLBACK:callback"
        }
        num_threads: 5
      )pb");

  // Starts MediaPipe graph.
  CalculatorGraph graph(graph_config);
  // Runs the graph twice.
  for (int i = 0; i < 2; ++i) {
    MP_ASSERT_OK(graph.StartRun(
        {{"callback", MakePacket<std::function<void(const Packet&)>>(std::bind(
                          &ParallelExecutionTest::AddThreadSafeVectorSink, this,
                          std::placeholders::_1))}}));
    const int kTotalNums = 100;
    int fail_count = 0;
    for (int i = 0; i < kTotalNums; ++i) {
      absl::Status status = graph.AddPacketToInputStream(
          "input", Adopt(new int(i)).At(Timestamp(i)));
      if (!status.ok()) {
        ++fail_count;
      }
    }

    EXPECT_EQ(0, fail_count);

    // Doesn't wait but just close the input stream.
    MP_ASSERT_OK(graph.CloseInputStream("input"));
    // Waits properly via the API until the graph is done.
    MP_ASSERT_OK(graph.WaitUntilDone());

    absl::ReaderMutexLock lock(&output_packets_mutex_);
    ASSERT_EQ(kTotalNums - kTotalNums / 4, output_packets_.size());
    int index = 1;
    for (const Packet& packet : output_packets_) {
      MP_ASSERT_OK(packet.ValidateAsType<int>());
      EXPECT_EQ(index + 2, packet.Get<int>());
      EXPECT_EQ(Timestamp(index), packet.Timestamp());
      if (++index % 4 == 0) {
        ++index;
      }
    }
    output_packets_.clear();
  }
}

}  // namespace
}  // namespace mediapipe
