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

// Executor class for the MediaPipe scheduler.

#ifndef MEDIAPIPE_FRAMEWORK_EXECUTOR_H_
#define MEDIAPIPE_FRAMEWORK_EXECUTOR_H_

#include <functional>

// TODO: Move protos in another CL after the C++ code migration.
#include "mediapipe/framework/deps/registration.h"
#include "mediapipe/framework/mediapipe_options.pb.h"
#include "mediapipe/framework/port/statusor.h"

namespace mediapipe {

// Abstract base class for the task queue.
// NOTE: The task queue orders the ready tasks by their priorities. This
// enables the executor to run ready tasks in priority order.
class TaskQueue {
 public:
  virtual ~TaskQueue();

  // Runs the next ready task in the current thread. Should be invoked by the
  // executor. This method should be called exactly as many times as AddTask
  // was called on the executor.
  virtual void RunNextTask() = 0;
};

// Abstract base class for the Executor.
class Executor {
 public:
  virtual ~Executor();

  // A registered Executor subclass must implement the static factory method
  // Create.  The Executor subclass cannot be registered without it.
  //
  // static absl::StatusOr<Executor*> Create(
  //     const MediaPipeOptions& extendable_options);
  //
  // Create validates extendable_options, then calls the constructor, and
  // returns the newly allocated Executor object.

  // The scheduler queue calls this method to tell the executor that it has
  // a new task to run. The executor should use its execution mechanism to
  // invoke task_queue->RunNextTask.
  virtual void AddTask(TaskQueue* task_queue) {
    Schedule([task_queue] { task_queue->RunNextTask(); });
  }

  // Schedule the specified "task" for execution in this executor.
  virtual void Schedule(std::function<void()> task) = 0;
};

using ExecutorRegistry =
    GlobalFactoryRegistry<absl::StatusOr<Executor*>, const MediaPipeOptions&>;

// Macro for registering the executor.
#define REGISTER_EXECUTOR(name)        \
  REGISTER_FACTORY_FUNCTION_QUALIFIED( \
      mediapipe::ExecutorRegistry, executor_registration, name, name::Create)

}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_EXECUTOR_H_
