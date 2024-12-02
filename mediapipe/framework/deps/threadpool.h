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

#ifndef MEDIAPIPE_DEPS_THREADPOOL_H_
#define MEDIAPIPE_DEPS_THREADPOOL_H_

#include <deque>
#include <functional>
#include <string>
#include <vector>

#include "absl/synchronization/mutex.h"
#include "mediapipe/framework/deps/thread_options.h"

namespace mediapipe {

// A thread pool consists of a set of threads that sit around waiting
// for callbacks to appear on a queue.  When that happens, one of the
// threads pulls a callback off the queue and runs it.
//
// The thread pool is shut down when the pool is destroyed.
//
// Sample usage:
//
// {
//   ThreadPool pool("testpool", num_workers);
//   pool.StartWorkers();
//   for (int i = 0; i < N; ++i) {
//     pool.Schedule([i]() { DoWork(i); });
//   }
// }
//
class ThreadPool {
 public:
  // Create a thread pool that provides a concurrency of "num_threads"
  // threads. I.e., if "num_threads" items are added, they are all
  // guaranteed to run concurrently without excessive delay.
  // It has an effectively infinite maximum queue length.
  // If num_threads is 1, the callbacks are run in FIFO order.
  explicit ThreadPool(int num_threads);
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;

  // Like the ThreadPool(int num_threads) constructor, except that
  // it also associates "name_prefix" with each of the threads
  // in the thread pool.
  ThreadPool(const std::string& name_prefix, int num_threads);

  // Create a thread pool that creates and can use up to "num_threads"
  // threads.  Any standard thread options, such as stack size, should
  // be passed via "thread_options".  "name_prefix" specifies the
  // thread name prefix.
  ThreadPool(const ThreadOptions& thread_options,
             const std::string& name_prefix, int num_threads);

  // Waits for closures (if any) to complete. May be called without
  // having called StartWorkers().
  ~ThreadPool();

  // REQUIRES: StartWorkers has not been called
  // Actually start the worker threads.
  void StartWorkers();

  // REQUIRES: StartWorkers has been called
  // Add specified callback to queue of pending callbacks.  Eventually a
  // thread will pull this callback off the queue and execute it.
  void Schedule(std::function<void()> callback);

  // Provided for debugging and testing only.
  int num_threads() const;

  // Standard thread options.  Use this accessor to get them.
  const ThreadOptions& thread_options() const;

 private:
  class WorkerThread;
  void RunWorker();

  std::string name_prefix_;
  std::vector<WorkerThread*> threads_;
  int num_threads_;

  absl::Mutex mutex_;
  absl::CondVar condition_;
  bool stopped_ ABSL_GUARDED_BY(mutex_) = false;
  std::deque<std::function<void()>> tasks_ ABSL_GUARDED_BY(mutex_);

  ThreadOptions thread_options_;
};

namespace internal {

// Creates name for thread in a thread pool based on provided prefix and
// thread id. Length of the resulting name is guaranteed to be less or equal
// to 15. Name or thread id can be truncated to achieve that, see truncation
// samples below:
// name_prefix, 1234       -> name_prefix/123
// name_prefix, 1234567    -> name_prefix/123
// name_prefix_long, 1234  -> name_prefix_lon
std::string CreateThreadName(const std::string& prefix, int thread_id);

}  // namespace internal

}  // namespace mediapipe

#endif  // MEDIAPIPE_DEPS_THREADPOOL_H_
