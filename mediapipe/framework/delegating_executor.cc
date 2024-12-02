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

#include "mediapipe/framework/delegating_executor.h"

#include <utility>

namespace mediapipe {
namespace internal {

void DelegatingExecutor::Schedule(std::function<void()> task) {
  callback_(std::move(task));
}

}  // namespace internal
}  // namespace mediapipe
