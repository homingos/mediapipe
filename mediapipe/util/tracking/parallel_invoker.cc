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

#include "mediapipe/util/tracking/parallel_invoker.h"

// Choose between ThreadPool, OpenMP and serial execution.
// Note only one parallel_using_* directive can be active.
int flags_parallel_invoker_mode = PARALLEL_INVOKER_MAX_VALUE;
int flags_parallel_invoker_max_threads = 4;

namespace mediapipe {

#if defined(PARALLEL_INVOKER_ACTIVE)
ThreadPool* ParallelInvokerThreadPool() {
  static ThreadPool* pool = []() -> ThreadPool* {
    ThreadPool* new_pool =
        new ThreadPool("ParallelInvoker", flags_parallel_invoker_max_threads);
    new_pool->StartWorkers();
    return new_pool;
  }();
  return pool;
}
#endif

}  // namespace mediapipe
