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

#include "mediapipe/framework/tool/simulation_clock.h"

#include "absl/synchronization/mutex.h"
#include "absl/time/time.h"
#include "mediapipe/framework/port/logging.h"

namespace mediapipe {

SimulationClock::~SimulationClock() {
  ThreadStart();
  ThreadFinish();
}

absl::Time SimulationClock::TimeNow() {
  absl::MutexLock l(&time_mutex_);
  return time_;
}

void SimulationClock::Sleep(absl::Duration d) {
  absl::MutexLock l(&time_mutex_);
  SleepInternal(time_ + d);
}

void SimulationClock::SleepUntil(absl::Time wakeup_time) {
  absl::MutexLock l(&time_mutex_);
  SleepInternal(wakeup_time);
}

void SimulationClock::SleepInternal(absl::Time wakeup_time) {
  Waiter waiter;
  waiters_.insert({wakeup_time, &waiter});
  num_running_--;
  TryAdvanceTime();
  while (waiter.sleeping) {
    waiter.cond.Wait(&time_mutex_);
  }
  num_running_++;
}

void SimulationClock::ThreadStart() {
  absl::MutexLock l(&time_mutex_);
  num_running_++;
}

void SimulationClock::ThreadFinish() {
  absl::MutexLock l(&time_mutex_);
  num_running_--;
  TryAdvanceTime();
}

void SimulationClock::TryAdvanceTime() {
  if (num_running_ == 0 && !waiters_.empty()) {
    VLOG(2) << "Advance time from: " << absl::ToUnixMicros(time_)
            << " to: " << absl::ToUnixMicros(waiters_.begin()->first);
    time_ = waiters_.begin()->first;
    Waiter* waiter = waiters_.begin()->second;
    waiters_.erase(waiters_.begin());
    waiter->sleeping = false;
    waiter->cond.Signal();
  }
}

}  // namespace mediapipe
