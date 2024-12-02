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

#ifndef MEDIAPIPE_DEPS_NUMBERS_H_
#define MEDIAPIPE_DEPS_NUMBERS_H_

#include <cstdint>
#include <string>

#include "absl/strings/numbers.h"
#include "absl/strings/str_cat.h"

namespace mediapipe {
ABSL_MUST_USE_RESULT inline std::string SimpleDtoa(double d) {
  if (static_cast<double>(static_cast<int64_t>(d)) == d) {
    return absl::StrCat(static_cast<int64_t>(d));
  } else {
    return absl::StrCat(d);
  }
}
}  // namespace mediapipe

#endif  // MEDIAPIPE_DEPS_NUMBERS_H_
