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
#include "mediapipe/framework/port/gmock.h"
#include "mediapipe/framework/port/gtest.h"

namespace mediapipe {
namespace {

static const char my_graph[] =
#include "mediapipe/framework/tool/test_binarypb.inc"
    ;  // NOLINT(whitespace/semicolon)

TEST(DataAsCString, CanDecodeCalculatorGraphConfig) {
  CalculatorGraphConfig config;
  bool success = config.ParseFromArray(my_graph, sizeof(my_graph) - 1);
  EXPECT_TRUE(success);
}

}  // namespace
}  // namespace mediapipe
