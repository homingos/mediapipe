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

#include <vector>

#include "mediapipe/framework/calculator_runner.h"
#include "mediapipe/framework/port/gmock.h"
#include "mediapipe/framework/port/gtest.h"
#include "mediapipe/framework/port/status_matchers.h"
#include "mediapipe/framework/tool/validate_type.h"

namespace mediapipe {
namespace {

inline Packet PacketFrom(int i) { return Adopt(new int(i)).At(Timestamp(i)); }

TEST(PacketInnerJoinCalculatorTest, AllMatching) {
  // Test case.
  const std::vector<int> packets_on_stream1 = {0, 1, 2, 3};
  const std::vector<int> packets_on_stream2 = {0, 1, 2, 3};
  // Run.
  CalculatorRunner runner("PacketInnerJoinCalculator", "", 2, 2, 0);
  for (int packet_load : packets_on_stream1) {
    runner.MutableInputs()->Index(0).packets.push_back(PacketFrom(packet_load));
  }
  for (int packet_load : packets_on_stream2) {
    runner.MutableInputs()->Index(1).packets.push_back(PacketFrom(packet_load));
  }
  MP_ASSERT_OK(runner.Run());
  // Check.
  const std::vector<int> expected = {0, 1, 2, 3};
  ASSERT_EQ(expected.size(), runner.Outputs().Index(0).packets.size());
  ASSERT_EQ(expected.size(), runner.Outputs().Index(1).packets.size());
  for (int i = 0; i < expected.size(); ++i) {
    const Packet packet1 = runner.Outputs().Index(0).packets[i];
    EXPECT_EQ(expected[i], packet1.Get<int>());
    EXPECT_EQ(expected[i], packet1.Timestamp().Value());
    const Packet packet2 = runner.Outputs().Index(1).packets[i];
    EXPECT_EQ(expected[i], packet2.Get<int>());
    EXPECT_EQ(expected[i], packet2.Timestamp().Value());
  }
}

TEST(PacketInnerJoinCalculatorTest, NoneMatching) {
  // Test case.
  const std::vector<int> packets_on_stream1 = {0, 2};
  const std::vector<int> packets_on_stream2 = {1, 3};
  // Run.
  CalculatorRunner runner("PacketInnerJoinCalculator", "", 2, 2, 0);
  for (int packet_load : packets_on_stream1) {
    runner.MutableInputs()->Index(0).packets.push_back(PacketFrom(packet_load));
  }
  for (int packet_load : packets_on_stream2) {
    runner.MutableInputs()->Index(1).packets.push_back(PacketFrom(packet_load));
  }
  MP_ASSERT_OK(runner.Run());
  // Check.
  EXPECT_TRUE(runner.Outputs().Index(0).packets.empty());
  EXPECT_TRUE(runner.Outputs().Index(1).packets.empty());
}

TEST(PacketInnerJoinCalculatorTest, SomeMatching) {
  // Test case.
  const std::vector<int> packets_on_stream1 = {0, 1, 2, 3, 4, 6};
  const std::vector<int> packets_on_stream2 = {0, 2, 4, 5, 6};
  // Run.
  CalculatorRunner runner("PacketInnerJoinCalculator", "", 2, 2, 0);
  for (int packet_load : packets_on_stream1) {
    runner.MutableInputs()->Index(0).packets.push_back(PacketFrom(packet_load));
  }
  for (int packet_load : packets_on_stream2) {
    runner.MutableInputs()->Index(1).packets.push_back(PacketFrom(packet_load));
  }
  MP_ASSERT_OK(runner.Run());
  // Check.
  const std::vector<int> expected = {0, 2, 4, 6};
  ASSERT_EQ(expected.size(), runner.Outputs().Index(0).packets.size());
  ASSERT_EQ(expected.size(), runner.Outputs().Index(1).packets.size());
  for (int i = 0; i < expected.size(); ++i) {
    const Packet packet1 = runner.Outputs().Index(0).packets[i];
    EXPECT_EQ(expected[i], packet1.Get<int>());
    EXPECT_EQ(expected[i], packet1.Timestamp().Value());
    const Packet packet2 = runner.Outputs().Index(1).packets[i];
    EXPECT_EQ(expected[i], packet2.Get<int>());
    EXPECT_EQ(expected[i], packet2.Timestamp().Value());
  }
}

}  // namespace
}  // namespace mediapipe
