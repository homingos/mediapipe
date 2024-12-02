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

#include "mediapipe/framework/deps/ret_check.h"

namespace mediapipe {

mediapipe::StatusBuilder RetCheckFailSlowPath(
    mediapipe::source_location location) {
  // TODO Implement LogWithStackTrace().
  return mediapipe::InternalErrorBuilder(location)
         << "RET_CHECK failure (" << location.file_name() << ":"
         << location.line() << ") ";
}

mediapipe::StatusBuilder RetCheckFailSlowPath(
    mediapipe::source_location location, const char* condition) {
  return mediapipe::RetCheckFailSlowPath(location) << condition;
}

mediapipe::StatusBuilder RetCheckFailSlowPath(
    mediapipe::source_location location, const char* condition,
    const absl::Status& status) {
  return mediapipe::RetCheckFailSlowPath(location)
         << condition << " returned " << status << " ";
}

}  // namespace mediapipe
