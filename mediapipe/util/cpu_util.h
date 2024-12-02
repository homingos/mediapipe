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

#ifndef MEDIAPIPE_UTIL_CPU_UTIL_H_
#define MEDIAPIPE_UTIL_CPU_UTIL_H_

#include <set>

namespace mediapipe {
// Returns the number of CPU cores. Compatible with Android.
int NumCPUCores();
// Returns a set of inferred CPU ids of lower cores.
std::set<int> InferLowerCoreIds();
// Returns a set of inferred CPU ids of higher cores.
std::set<int> InferHigherCoreIds();
}  // namespace mediapipe

#endif  // MEDIAPIPE_UTIL_CPU_UTIL_H_
