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

#ifndef MEDIAPIPE_GPU_GL_THREAD_COLLECTOR_H_
#define MEDIAPIPE_GPU_GL_THREAD_COLLECTOR_H_

#include <cstdlib>

#if defined(MEDIAPIPE_USING_LEGACY_SWIFTSHADER)
#define MEDIAPIPE_NEEDS_GL_THREAD_COLLECTOR 1
#endif

#if MEDIAPIPE_NEEDS_GL_THREAD_COLLECTOR
#include "absl/synchronization/mutex.h"
#include "mediapipe/framework/deps/no_destructor.h"
#endif  // MEDIAPIPE_NEEDS_GL_THREAD_COLLECTOR

namespace mediapipe {

#if MEDIAPIPE_NEEDS_GL_THREAD_COLLECTOR

class GlThreadCollector {
 public:
  static void ThreadStarting() { Collector().ChangeCount(1); }

  static void ThreadEnding() { Collector().ChangeCount(-1); }

 private:
  GlThreadCollector() { std::atexit(WaitForThreadsToTerminate); }

  static GlThreadCollector& Collector() {
    static NoDestructor<GlThreadCollector> collector;
    return *collector;
  }

  static void WaitForThreadsToTerminate() { Collector().Wait(); }

  void ChangeCount(int delta) {
    absl::MutexLock l(&mutex_);
    active_threads_ += delta;
  }

  void Wait() {
    auto done = [this]() {
      mutex_.AssertReaderHeld();
      return active_threads_ == 0;
    };
    absl::MutexLock l(&mutex_);
    mutex_.Await(absl::Condition(&done));
  }

  absl::Mutex mutex_;
  int active_threads_ ABSL_GUARDED_BY(mutex_) = 0;
  friend NoDestructor<GlThreadCollector>;
};
#else
class GlThreadCollector {
 public:
  static void ThreadStarting() {}
  static void ThreadEnding() {}
};
#endif  // MEDIAPIPE_NEEDS_GL_THREAD_COLLECTOR

}  // namespace mediapipe

#endif  // MEDIAPIPE_GPU_GL_THREAD_COLLECTOR_H_
