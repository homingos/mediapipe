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

#include "mediapipe/framework/calculator_runner.h"
#include "mediapipe/framework/port/gmock.h"
#include "mediapipe/framework/port/gtest.h"
#include "mediapipe/framework/port/status_matchers.h"
#include "mediapipe/framework/tool/validate_type.h"

namespace mediapipe {

namespace {

constexpr char kPacketOffsetTag[] = "PACKET_OFFSET";

// Adds packets containing integers equal to their original timestamp.
void AddPackets(CalculatorRunner* runner) {
  for (int i = 0; i < 10; ++i) {
    runner->MutableInputs()->Index(0).packets.push_back(
        Adopt(new int(i)).At(Timestamp(i)));
  }
}

// Zero shift is a no-op (output input[i] at timestamp[i]). Input and output
// streams should be identical.
TEST(SequenceShiftCalculatorTest, ZeroShift) {
  CalculatorRunner runner(
      "SequenceShiftCalculator",
      "[mediapipe.SequenceShiftCalculatorOptions.ext]: { packet_offset: 0 }", 1,
      1, 0);
  AddPackets(&runner);
  MP_ASSERT_OK(runner.Run());
  const std::vector<Packet>& input_packets =
      runner.MutableInputs()->Index(0).packets;
  const std::vector<Packet>& output_packets = runner.Outputs().Index(0).packets;
  ASSERT_EQ(10, input_packets.size());
  ASSERT_EQ(input_packets.size(), output_packets.size());
  for (int i = 0; i < output_packets.size(); ++i) {
    // Make sure the contents are as expected.
    EXPECT_EQ(input_packets[i].Get<int>(), output_packets[i].Get<int>());
    EXPECT_EQ(input_packets[i].Timestamp(), output_packets[i].Timestamp());
  }
}

// Tests shifting by three packets, i.e., output input[i] with the timestamp of
// input[i + 3].
TEST(SequenceShiftCalculatorTest, PositiveShift) {
  CalculatorRunner runner(
      "SequenceShiftCalculator",
      "[mediapipe.SequenceShiftCalculatorOptions.ext]: { packet_offset: 3 }", 1,
      1, 0);
  AddPackets(&runner);
  MP_ASSERT_OK(runner.Run());
  const std::vector<Packet>& input_packets =
      runner.MutableInputs()->Index(0).packets;
  const std::vector<Packet>& output_packets = runner.Outputs().Index(0).packets;
  ASSERT_EQ(10, input_packets.size());
  // input_packet[i] should be output with the timestamp of input_packet[i + 3].
  // The last 3 packets are dropped.
  ASSERT_EQ(7, output_packets.size());
  for (int i = 0; i < output_packets.size(); ++i) {
    // Make sure the contents are as expected.
    EXPECT_EQ(input_packets[i].Get<int>(), output_packets[i].Get<int>());
    // Make sure the timestamps are shifted as expected.
    EXPECT_EQ(input_packets[i + 3].Timestamp(), output_packets[i].Timestamp());
  }
}

// Tests shifting by -2, i.e., output input[i] with timestamp[i - 2]. The first
// two packets should be dropped.
TEST(SequenceShiftCalculatorTest, NegativeShift) {
  CalculatorRunner runner(
      "SequenceShiftCalculator",
      "[mediapipe.SequenceShiftCalculatorOptions.ext]: { packet_offset: -2 }",
      1, 1, 0);
  AddPackets(&runner);
  MP_ASSERT_OK(runner.Run());
  const std::vector<Packet>& input_packets =
      runner.MutableInputs()->Index(0).packets;
  const std::vector<Packet>& output_packets = runner.Outputs().Index(0).packets;
  ASSERT_EQ(10, input_packets.size());
  // Input packet[i] should be output with the timestamp of input packet[i - 2].
  // The first two packets are dropped. This means timestamps match between
  // input and output packets, but the data in the output packets come from
  // input_packets[i + 2].
  ASSERT_EQ(8, output_packets.size());
  for (int i = 0; i < output_packets.size(); ++i) {
    EXPECT_EQ(input_packets[i].Timestamp(), output_packets[i].Timestamp());
    EXPECT_EQ(input_packets[i + 2].Get<int>(), output_packets[i].Get<int>());
  }
}

// Tests using a side packet to specify the offset.  Shifting by -2, i.e.,
// output input[i] with timestamp[i - 2]. The first two packets should be
// dropped.
TEST(SequenceShiftCalculatorTest, SidePacketOffset) {
  CalculatorGraphConfig::Node node;
  node.set_calculator("SequenceShiftCalculator");
  node.add_input_stream("input");
  node.add_output_stream("output");
  node.add_input_side_packet("PACKET_OFFSET:packet_offset");

  CalculatorRunner runner(node);
  AddPackets(&runner);
  runner.MutableSidePackets()->Tag(kPacketOffsetTag) = Adopt(new int(-2));
  MP_ASSERT_OK(runner.Run());
  const std::vector<Packet>& input_packets =
      runner.MutableInputs()->Index(0).packets;
  const std::vector<Packet>& output_packets = runner.Outputs().Index(0).packets;
  ASSERT_EQ(10, input_packets.size());
  // Input packet[i] should be output with the timestamp of input packet[i - 2].
  // The first two packets are dropped. This means timestamps match between
  // input and output packets, but the data in the output packets come from
  // input_packets[i + 2].
  ASSERT_EQ(8, output_packets.size());
  for (int i = 0; i < output_packets.size(); ++i) {
    EXPECT_EQ(input_packets[i].Timestamp(), output_packets[i].Timestamp());
    EXPECT_EQ(input_packets[i + 2].Get<int>(), output_packets[i].Get<int>());
  }
}

}  // namespace

}  // namespace mediapipe
