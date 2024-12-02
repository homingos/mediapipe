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

#include "mediapipe/framework/deps/status.h"

#include <stdio.h>

namespace mediapipe {

std::ostream& operator<<(std::ostream& os, const absl::Status& x) {
  os << x.ToString();
  return os;
}

std::string* MediaPipeCheckOpHelperOutOfLine(const absl::Status& v,
                                             const char* msg) {
  std::string r("Non-OK-status: ");
  r += msg;
  r += " status: ";
  r += v.ToString();
  // Leaks string but this is only to be used in a fatal error message
  return new std::string(r);
}

}  // namespace mediapipe
