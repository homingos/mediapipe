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

#ifndef MEDIAPIPE_FRAMEWORK_SCHEDULER_SHARED_H_
#define MEDIAPIPE_FRAMEWORK_SCHEDULER_SHARED_H_

#include <atomic>
#include <cstdint>
#include <functional>
#include <memory>
#include <queue>
#include <utility>

#include "absl/base/macros.h"
#include "absl/synchronization/mutex.h"
#include "mediapipe/framework/deps/clock.h"
#include "mediapipe/framework/deps/monotonic_clock.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {
namespace internal {

// This is meant for testing purposes only.
struct SchedulerTimes {
  // Total run time measured by the scheduler, in microseconds.
  int64_t total_time;
  // Total time spent running nodes, in microseconds.
  int64_t node_time;
  // The fraction of total time which was not spent running nodes. Only valid
  // when the graph is run on a single thread.
  double overhead() const {
    return 1.0 -
           (static_cast<double>(node_time) / static_cast<double>(total_time));
  }
};

// This service class is used to compute the scheduler overhead.
// It keeps track of the total runtime of the graph, and of the total time
// spent actually running nodes. The difference is considered overhead.
// This is a crude measure that only makes sense when running with a single
// worker thread, but it has the advantage of being simple and continuing to
// work across the scheduler changes we want to test.
class SchedulerTimer {
 public:
  SchedulerTimer() {
    clock_ = std::unique_ptr<mediapipe::Clock>(
        mediapipe::MonotonicClock::CreateSynchronizedMonotonicClock());
  }

  // Called when starting the scheduler.
  void StartRun() {
    start_time_ = absl::ToUnixMicros(clock_->TimeNow());
    total_node_time_ = 0;
  }
  // Called when terminating the scheduler.
  void EndRun() {
    total_run_time_ = absl::ToUnixMicros(clock_->TimeNow()) - start_time_;
  }

  // Called immediately before invoking ProcessNode or CloseNode.
  int64_t StartNode() { return absl::ToUnixMicros(clock_->TimeNow()); }
  // Called immediately after invoking ProcessNode or CloseNode.
  void EndNode(int64_t node_start_time) {
    total_node_time_.fetch_add(
        absl::ToUnixMicros(clock_->TimeNow()) - node_start_time,
        std::memory_order_relaxed);
  }

  SchedulerTimes GetSchedulerTimes() {
    internal::SchedulerTimes result;
    result.total_time = total_run_time_;
    result.node_time = total_node_time_;
    return result;
  }

 private:
  // Timer for measuring overhead.
  std::unique_ptr<mediapipe::Clock> clock_;

  // Time spent actually running nodes, in microseconds.
  std::atomic<int64_t> total_node_time_;

  // The start time of the graph, in microseconds.
  int64_t start_time_;
  // Total time spent running the graph, in microseconds.
  int64_t total_run_time_;
};

struct SchedulerShared {
  // When a non-source node returns StatusStop() or
  // CalculatorGraph::CloseAllPacketSources is called, the graph starts to
  // terminate: all source nodes are closed (at the next scheduling
  // opportunity), and the graph continues running until it is done. This
  // flag indicates that the graph is in that mode.
  std::atomic<bool> stopping;
  std::atomic<bool> has_error;
  std::function<void(const absl::Status& error)> error_callback;
  // Collects timing information for measuring overhead.
  internal::SchedulerTimer timer;
};

}  // namespace internal
}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_SCHEDULER_SHARED_H_
