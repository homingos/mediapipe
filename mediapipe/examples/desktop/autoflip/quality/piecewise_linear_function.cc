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

#include <algorithm>
#include <limits>
#include <vector>

#include "absl/log/absl_check.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {
namespace autoflip {

void PiecewiseLinearFunction::AddPoint(double x, double y) {
  if (!points_.empty()) {
    ABSL_CHECK_GE(x, points_.back().x)
        << "Points must be provided in non-decreasing x order.";
  }
  points_.push_back(PiecewiseLinearFunction::Point(x, y));
}

std::vector<PiecewiseLinearFunction::Point>::const_iterator
PiecewiseLinearFunction::GetIntervalIterator(double input) const {
  PiecewiseLinearFunction::Point input_point(input, 0);
  std::vector<PiecewiseLinearFunction::Point>::const_iterator iter =
      std::lower_bound(points_.begin(), points_.end(), input_point,
                       PointCompare());
  return iter;
}

double PiecewiseLinearFunction::Interpolate(
    const PiecewiseLinearFunction::Point& p1,
    const PiecewiseLinearFunction::Point& p2, double input) const {
  ABSL_CHECK_LT(p1.x, input);
  ABSL_CHECK_GE(p2.x, input);

  return p2.y - (p2.x - input) / (p2.x - p1.x) * (p2.y - p1.y);
}

double PiecewiseLinearFunction::Evaluate(double const input) const {
  std::vector<PiecewiseLinearFunction::Point>::const_iterator i =
      GetIntervalIterator(input);
  if (i == points_.begin()) {
    return points_.front().y;
  }
  if (i == points_.end()) {
    return points_.back().y;
  }

  std::vector<PiecewiseLinearFunction::Point>::const_iterator prev = i - 1;
  return Interpolate(*prev, *i, input);
}

}  // namespace autoflip
}  // namespace mediapipe
