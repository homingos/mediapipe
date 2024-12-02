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

#include "mediapipe/framework/tool/simulation_clock_executor.h"

#include "mediapipe/framework/tool/simulation_clock.h"

namespace mediapipe {

SimulationClockExecutor::SimulationClockExecutor(int num_threads)
    : clock_(new SimulationClock()), executor_(num_threads) {}

void SimulationClockExecutor::Schedule(std::function<void()> task) {
  clock_->ThreadStart();
  executor_.Schedule([this, task] {
    clock_->Sleep(absl::ZeroDuration());
    task();
    clock_->ThreadFinish();
  });
}

std::shared_ptr<SimulationClock> SimulationClockExecutor::GetClock() {
  return clock_;
}

}  // namespace mediapipe
