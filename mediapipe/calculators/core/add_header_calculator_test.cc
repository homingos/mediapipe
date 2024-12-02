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

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/calculator_runner.h"
#include "mediapipe/framework/port/canonical_errors.h"
#include "mediapipe/framework/port/gmock.h"
#include "mediapipe/framework/port/gtest.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/port/status_matchers.h"
#include "mediapipe/framework/timestamp.h"
#include "mediapipe/framework/tool/validate_type.h"

namespace mediapipe {

constexpr char kDataTag[] = "DATA";
constexpr char kHeaderTag[] = "HEADER";

class AddHeaderCalculatorTest : public ::testing::Test {};

TEST_F(AddHeaderCalculatorTest, HeaderStream) {
  CalculatorGraphConfig::Node node;
  node.set_calculator("AddHeaderCalculator");
  node.add_input_stream("HEADER:header_stream");
  node.add_input_stream("DATA:data_stream");
  node.add_output_stream("merged_stream");

  CalculatorRunner runner(node);

  // Set header and add 5 packets.
  runner.MutableInputs()->Tag(kHeaderTag).header =
      Adopt(new std::string("my_header"));
  for (int i = 0; i < 5; ++i) {
    Packet packet = Adopt(new int(i)).At(Timestamp(i * 1000));
    runner.MutableInputs()->Tag(kDataTag).packets.push_back(packet);
  }

  // Run calculator.
  MP_ASSERT_OK(runner.Run());

  ASSERT_EQ(1, runner.Outputs().NumEntries());

  // Test output.
  EXPECT_EQ(std::string("my_header"),
            runner.Outputs().Index(0).header.Get<std::string>());
  const std::vector<Packet>& output_packets = runner.Outputs().Index(0).packets;
  ASSERT_EQ(5, output_packets.size());
  for (int i = 0; i < 5; ++i) {
    const int val = output_packets[i].Get<int>();
    EXPECT_EQ(i, val);
    EXPECT_EQ(Timestamp(i * 1000), output_packets[i].Timestamp());
  }
}

TEST_F(AddHeaderCalculatorTest, HandlesEmptyHeaderStream) {
  CalculatorGraphConfig::Node node;
  node.set_calculator("AddHeaderCalculator");
  node.add_input_stream("HEADER:header_stream");
  node.add_input_stream("DATA:data_stream");
  node.add_output_stream("merged_stream");

  CalculatorRunner runner(node);

  // No header and no packets.
  // Run calculator.
  MP_ASSERT_OK(runner.Run());
  EXPECT_TRUE(runner.Outputs().Index(0).header.IsEmpty());
}

TEST_F(AddHeaderCalculatorTest, NoPacketsOnHeaderStream) {
  CalculatorGraphConfig::Node node;
  node.set_calculator("AddHeaderCalculator");
  node.add_input_stream("HEADER:header_stream");
  node.add_input_stream("DATA:data_stream");
  node.add_output_stream("merged_stream");

  CalculatorRunner runner(node);

  // Set header and add 5 packets.
  runner.MutableInputs()->Tag(kHeaderTag).header =
      Adopt(new std::string("my_header"));
  runner.MutableInputs()
      ->Tag(kHeaderTag)
      .packets.push_back(Adopt(new std::string("not allowed")));
  for (int i = 0; i < 5; ++i) {
    Packet packet = Adopt(new int(i)).At(Timestamp(i * 1000));
    runner.MutableInputs()->Tag(kDataTag).packets.push_back(packet);
  }

  // Run calculator.
  ASSERT_FALSE(runner.Run().ok());
}

TEST_F(AddHeaderCalculatorTest, InputSidePacket) {
  CalculatorGraphConfig::Node node;
  node.set_calculator("AddHeaderCalculator");
  node.add_input_stream("DATA:data_stream");
  node.add_output_stream("merged_stream");
  node.add_input_side_packet("HEADER:header");

  CalculatorRunner runner(node);

  // Set header and add 5 packets.
  runner.MutableSidePackets()->Tag(kHeaderTag) =
      Adopt(new std::string("my_header"));
  for (int i = 0; i < 5; ++i) {
    Packet packet = Adopt(new int(i)).At(Timestamp(i * 1000));
    runner.MutableInputs()->Tag(kDataTag).packets.push_back(packet);
  }

  // Run calculator.
  MP_ASSERT_OK(runner.Run());

  ASSERT_EQ(1, runner.Outputs().NumEntries());

  // Test output.
  EXPECT_EQ(std::string("my_header"),
            runner.Outputs().Index(0).header.Get<std::string>());
  const std::vector<Packet>& output_packets = runner.Outputs().Index(0).packets;
  ASSERT_EQ(5, output_packets.size());
  for (int i = 0; i < 5; ++i) {
    const int val = output_packets[i].Get<int>();
    EXPECT_EQ(i, val);
    EXPECT_EQ(Timestamp(i * 1000), output_packets[i].Timestamp());
  }
}

TEST_F(AddHeaderCalculatorTest, UsingBothSideInputAndStream) {
  CalculatorGraphConfig::Node node;
  node.set_calculator("AddHeaderCalculator");
  node.add_input_stream("HEADER:header_stream");
  node.add_input_stream("DATA:data_stream");
  node.add_output_stream("merged_stream");
  node.add_input_side_packet("HEADER:header");

  CalculatorRunner runner(node);

  // Set both headers and add 5 packets.
  runner.MutableSidePackets()->Tag(kHeaderTag) =
      Adopt(new std::string("my_header"));
  runner.MutableSidePackets()->Tag(kHeaderTag) =
      Adopt(new std::string("my_header"));
  for (int i = 0; i < 5; ++i) {
    Packet packet = Adopt(new int(i)).At(Timestamp(i * 1000));
    runner.MutableInputs()->Tag(kDataTag).packets.push_back(packet);
  }

  // Run should fail because header can only be provided one way.
  EXPECT_EQ(runner.Run().code(), absl::InvalidArgumentError("").code());
}

}  // namespace mediapipe
