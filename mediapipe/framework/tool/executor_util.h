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

#ifndef MEDIAPIPE_FRAMEWORK_TOOL_EXECUTOR_UTIL_H_
#define MEDIAPIPE_FRAMEWORK_TOOL_EXECUTOR_UTIL_H_

#include <cstdint>

#include "mediapipe/framework/calculator.pb.h"

namespace mediapipe {

namespace tool {
// Ensures the default executor's stack size is at least min_stack_size.
//
// Note that this will also initialize the default executor; any configuration
// changes, such as num_threads, should be done to the config before calling
// this.
void EnsureMinimumDefaultExecutorStackSize(int32_t min_stack_size,
                                           CalculatorGraphConfig* config);
}  // namespace tool
}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_TOOL_EXECUTOR_UTIL_H_
