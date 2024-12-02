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

#ifndef MEDIAPIPE_FRAMEWORK_DELEGATING_EXECUTOR_H_
#define MEDIAPIPE_FRAMEWORK_DELEGATING_EXECUTOR_H_

#include "mediapipe/framework/executor.h"

namespace mediapipe {
namespace internal {

// An executor that delegates the running of tasks using a callback.
class DelegatingExecutor : public Executor {
 public:
  explicit DelegatingExecutor(
      std::function<void(std::function<void()>)> callback)
      : callback_(std::move(callback)) {}
  void Schedule(std::function<void()> task) override;

 private:
  std::function<void(std::function<void()>)> callback_;
};

}  // namespace internal
}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_DELEGATING_EXECUTOR_H_
