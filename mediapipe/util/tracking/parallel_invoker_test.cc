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

#include <algorithm>
#include <numeric>

#include "absl/synchronization/mutex.h"
#include "mediapipe/framework/port/gtest.h"

namespace mediapipe {
namespace {

void RunParallelTest() {
  absl::Mutex numbers_mutex;
  std::vector<int> numbers;
  const int kArraySize = 5000;

  // Fill number array in parallel.
  ParallelFor(0, kArraySize, 1,
              [&numbers_mutex, &numbers](const BlockedRange& b) {
                for (int k = b.begin(); k != b.end(); ++k) {
                  absl::MutexLock lock(&numbers_mutex);
                  numbers.push_back(k);
                }
              });

  std::vector<int> expected(kArraySize);
  std::iota(expected.begin(), expected.end(), 0);
  EXPECT_TRUE(
      std::is_permutation(expected.begin(), expected.end(), numbers.begin()));
}

TEST(ParallelInvokerTest, PhotosTest) {
  flags_parallel_invoker_mode = PARALLEL_INVOKER_OPENMP;

  RunParallelTest();
}

TEST(ParallelInvokerTest, ThreadPoolTest) {
  flags_parallel_invoker_mode = PARALLEL_INVOKER_THREAD_POOL;

  // Needs to be run in opt mode to pass.
  RunParallelTest();
}

}  // namespace
}  // namespace mediapipe
