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

#include "mediapipe/examples/desktop/autoflip/quality/piecewise_linear_function.h"

#include <stddef.h>

#include <memory>

#include "mediapipe/framework/port/gtest.h"
#include "mediapipe/framework/port/status.h"

namespace {

using mediapipe::autoflip::PiecewiseLinearFunction;

// It should be OK to pass a spec that's out of order as it gets sorted.
TEST(PiecewiseLinearFunctionTest, ReordersSpec) {
  PiecewiseLinearFunction f;
  // This defines the line y = x between 0 and 5
  f.AddPoint(0, 0);
  f.AddPoint(1, 1);
  f.AddPoint(2, 2);
  f.AddPoint(3, 3);
  f.AddPoint(5, 5);

  // Should be 0 as -1 is less than the smallest x value in the spec so it
  // should saturate.
  ASSERT_EQ(0, f.Evaluate(-1));

  // These shoud all be on the line y = x
  ASSERT_EQ(0, f.Evaluate(0));
  ASSERT_EQ(0.5, f.Evaluate(0.5));
  ASSERT_EQ(4.5, f.Evaluate(4.5));
  ASSERT_EQ(5, f.Evaluate(5));

  // Saturating on the high end.
  ASSERT_EQ(5, f.Evaluate(6));
}

TEST(PiecewiseLinearFunctionTest, TestAddPoints) {
  PiecewiseLinearFunction function;
  function.AddPoint(0.0, 0.0);
  function.AddPoint(1.0, 1.0);
  EXPECT_DOUBLE_EQ(0.0, function.Evaluate(-1.0));
  EXPECT_DOUBLE_EQ(0.0, function.Evaluate(0.0));
  EXPECT_DOUBLE_EQ(0.25, function.Evaluate(0.25));
}

TEST(PiecewiseLinearFunctionTest, AddPointsDiscontinuous) {
  PiecewiseLinearFunction function;
  function.AddPoint(-1.0, 0.0);
  function.AddPoint(0.0, 0.0);
  function.AddPoint(0.0, 1.0);
  function.AddPoint(1.0, 1.0);
  EXPECT_DOUBLE_EQ(0.0, function.Evaluate(-1.0));
  EXPECT_DOUBLE_EQ(0.0, function.Evaluate(0.0));
  EXPECT_DOUBLE_EQ(1.0, function.Evaluate(1e-12));
  EXPECT_DOUBLE_EQ(1.0, function.Evaluate(3.14));
}

}  // namespace
