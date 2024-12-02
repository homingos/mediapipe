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

#ifndef MEDIAPIPE_FRAMEWORK_PROFILER_PROFILER_RESOURCE_UTIL_H_
#define MEDIAPIPE_FRAMEWORK_PROFILER_PROFILER_RESOURCE_UTIL_H_

#include <string>

#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/port/statusor.h"

namespace mediapipe {

// Returns the path to the directory where trace logs will be stored by default.
// If the function is unable to find an appropriate directory, it returns an
// error.
StatusOr<std::string> GetDefaultTraceLogDirectory();

// Given a log file path, this function provides an absolute path with which
// it can be accessed as a file.  Enclosing directories are created as needed.
StatusOr<std::string> PathToLogFile(const std::string& path);

}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_PROFILER_PROFILER_RESOURCE_UTIL_H_
