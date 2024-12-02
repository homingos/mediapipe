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

#ifndef MEDIAPIPE_EXAMPLES_DESKTOP_AUTOFLIP_QUALITY_MATH_UTILS_H_
#define MEDIAPIPE_EXAMPLES_DESKTOP_AUTOFLIP_QUALITY_MATH_UTILS_H_

class MathUtil {
 public:
  // Clamps value to the range [low, high].  Requires low <= high. Returns false
  // if this check fails, otherwise returns true. Caller should first check the
  // returned boolean.
  template <typename T>  // T models LessThanComparable.
  static bool Clamp(const T& low, const T& high, const T& value, T* result) {
    // Prevents errors in ordering the arguments.
    if (low > high) {
      return false;
    }
    if (high < value) {
      *result = high;
    } else if (value < low) {
      *result = low;
    } else {
      *result = value;
    }
    return true;
  }
};

#endif  // MEDIAPIPE_EXAMPLES_DESKTOP_AUTOFLIP_QUALITY_MATH_UTILS_H_
