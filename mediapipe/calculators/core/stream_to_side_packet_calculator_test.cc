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

#include <memory>
#include <string>

#include "absl/memory/memory.h"
#include "mediapipe/framework/calculator_runner.h"
#include "mediapipe/framework/packet.h"
#include "mediapipe/framework/port/gmock.h"
#include "mediapipe/framework/port/gtest.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/port/status_matchers.h"
#include "mediapipe/framework/timestamp.h"

namespace mediapipe {

using ::testing::Test;

class StreamToSidePacketCalculatorTest : public Test {
 protected:
  StreamToSidePacketCalculatorTest() {
    const char kConfig[] = R"(
      calculator: "StreamToSidePacketCalculator"
      input_stream: "stream"
      output_side_packet: "side_packet"
    )";
    runner_ = absl::make_unique<CalculatorRunner>(kConfig);
  }

  std::unique_ptr<CalculatorRunner> runner_;
};

TEST_F(StreamToSidePacketCalculatorTest,
       StreamToSidePacketCalculatorWithEmptyStreamFails) {
  EXPECT_EQ(runner_->Run().code(), absl::StatusCode::kUnavailable);
}

TEST_F(StreamToSidePacketCalculatorTest,
       StreamToSidePacketCalculatorWithSinglePacketCreatesSidePacket) {
  runner_->MutableInputs()->Index(0).packets.push_back(
      Adopt(new std::string("test")).At(Timestamp(1)));
  MP_ASSERT_OK(runner_->Run());
  EXPECT_EQ(runner_->OutputSidePackets().Index(0).Get<std::string>(), "test");
}

TEST_F(StreamToSidePacketCalculatorTest,
       StreamToSidePacketCalculatorWithMultiplePacketsFails) {
  runner_->MutableInputs()->Index(0).packets.push_back(
      Adopt(new std::string("test1")).At(Timestamp(1)));
  runner_->MutableInputs()->Index(0).packets.push_back(
      Adopt(new std::string("test2")).At(Timestamp(2)));
  EXPECT_EQ(runner_->Run().code(), absl::StatusCode::kAlreadyExists);
}

}  // namespace mediapipe
