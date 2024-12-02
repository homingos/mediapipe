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

#include "mediapipe/framework/thread_pool_executor.h"

#include <utility>

#include "mediapipe/framework/port/canonical_errors.h"
#include "mediapipe/framework/port/logging.h"
#include "mediapipe/framework/port/status_builder.h"
#include "mediapipe/framework/thread_pool_executor.pb.h"
#include "mediapipe/util/cpu_util.h"

namespace mediapipe {

// static
absl::StatusOr<Executor*> ThreadPoolExecutor::Create(
    const MediaPipeOptions& extendable_options) {
  auto& options =
      extendable_options.GetExtension(ThreadPoolExecutorOptions::ext);
  if (!options.has_num_threads()) {
    return absl::InvalidArgumentError(
        "num_threads is not specified in ThreadPoolExecutorOptions.");
  }
  if (options.num_threads() <= 0) {
    return mediapipe::InvalidArgumentErrorBuilder(MEDIAPIPE_LOC)
           << "The num_threads field in ThreadPoolExecutorOptions should be "
              "positive but is "
           << options.num_threads();
  }

  ThreadOptions thread_options;
  if (options.has_stack_size()) {
    // thread_options.set_stack_size() takes a size_t as input, so we must not
    // pass a negative value. 0 has a special meaning (the default thread
    // stack size for the system), so we also avoid that.
    if (options.stack_size() <= 0) {
      return mediapipe::InvalidArgumentErrorBuilder(MEDIAPIPE_LOC)
             << "The stack_size field in ThreadPoolExecutorOptions should be "
                "positive but is "
             << options.stack_size();
    }
    thread_options.set_stack_size(options.stack_size());
  }
  if (options.has_nice_priority_level()) {
    thread_options.set_nice_priority_level(options.nice_priority_level());
  }
  if (options.has_thread_name_prefix()) {
    thread_options.set_name_prefix(options.thread_name_prefix());
  }
#if defined(__linux__)
  switch (options.require_processor_performance()) {
    case ThreadPoolExecutorOptions::LOW:
      thread_options.set_cpu_set(InferLowerCoreIds());
      break;
    case ThreadPoolExecutorOptions::HIGH:
      thread_options.set_cpu_set(InferHigherCoreIds());
      break;
    default:
      break;
  }
#endif
  return new ThreadPoolExecutor(thread_options, options.num_threads());
}

ThreadPoolExecutor::ThreadPoolExecutor(int num_threads)
    : thread_pool_("mediapipe", num_threads) {
  Start();
}

ThreadPoolExecutor::ThreadPoolExecutor(const ThreadOptions& thread_options,
                                       int num_threads)
    : thread_pool_(thread_options,
                   thread_options.name_prefix().empty()
                       ? "mediapipe"
                       : thread_options.name_prefix(),
                   num_threads) {
  Start();
}

ThreadPoolExecutor::~ThreadPoolExecutor() {
  VLOG(2) << "Terminating thread pool.";
}

void ThreadPoolExecutor::Schedule(std::function<void()> task) {
  thread_pool_.Schedule(std::move(task));
}

void ThreadPoolExecutor::Start() {
  stack_size_ = thread_pool_.thread_options().stack_size();
  thread_pool_.StartWorkers();
  VLOG(2) << "Started thread pool with " << thread_pool_.num_threads()
          << " threads.";
}

REGISTER_EXECUTOR(ThreadPoolExecutor);

}  // namespace mediapipe
