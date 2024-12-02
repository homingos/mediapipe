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

#ifndef MEDIAPIPE_FRAMEWORK_THREAD_POOL_EXECUTOR_H_
#define MEDIAPIPE_FRAMEWORK_THREAD_POOL_EXECUTOR_H_

#include "mediapipe/framework/deps/thread_options.h"
#include "mediapipe/framework/executor.h"
#include "mediapipe/framework/port/statusor.h"
#include "mediapipe/framework/port/threadpool.h"

namespace mediapipe {

// A multithreaded executor based on a thread pool.
class ThreadPoolExecutor : public Executor {
 public:
  static absl::StatusOr<Executor*> Create(
      const MediaPipeOptions& extendable_options);

  explicit ThreadPoolExecutor(int num_threads);
  ~ThreadPoolExecutor() override;
  void Schedule(std::function<void()> task) override;

  // For testing.
  int num_threads() const { return thread_pool_.num_threads(); }
  // Returns the thread stack size (in bytes).
  size_t stack_size() const { return stack_size_; }

 private:
  ThreadPoolExecutor(const ThreadOptions& thread_options, int num_threads);

  // Saves the value of the stack size option and starts the thread pool.
  void Start();

  mediapipe::ThreadPool thread_pool_;

  // Records the stack size in ThreadOptions right before we call
  // thread_pool_.StartWorkers().
  //
  // The actual stack size passed to pthread_attr_setstacksize() for the
  // worker threads differs from the stack size we specified. It includes the
  // guard size and space for thread-local storage. (See Thread::Start() in
  // thread/thread.cc.) So the unit tests check the stack size in
  // ThreadOptions, in addition to trying to recover the specified stack
  // size from the stack size returned by pthread_getattr_np(),
  // pthread_attr_getstacksize(), and pthread_attr_getguardsize().
  size_t stack_size_ = 0;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_THREAD_POOL_EXECUTOR_H_
