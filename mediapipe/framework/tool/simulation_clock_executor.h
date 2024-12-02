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

#ifndef MEDIAPIPE_FRAMEWORK_TOOL_SIMULATION_CLOCK_EXECUTOR_H_
#define MEDIAPIPE_FRAMEWORK_TOOL_SIMULATION_CLOCK_EXECUTOR_H_

#include "mediapipe/framework/thread_pool_executor.h"
#include "mediapipe/framework/tool/simulation_clock.h"

namespace mediapipe {

// Simulation clock multithreaded executor. This is intended to be used with
// graphs that are using SimulationClock class to emulate various parts of the
// graph taking specific time to process the incoming packets.
class SimulationClockExecutor : public Executor {
 public:
  explicit SimulationClockExecutor(int num_threads);
  void Schedule(std::function<void()> task) override;

  // Returns a pointer to the instance of SimulationClock used by
  // this executor. This instance can be passed down to graph nodes as input
  // side packet.
  std::shared_ptr<SimulationClock> GetClock();

 private:
  // SimulationClock instance used by this executor.
  std::shared_ptr<SimulationClock> clock_;
  // The delegate ThreadPoolExecutor.  This is declared after clock_
  // so that it is destroyed before clock_.
  ThreadPoolExecutor executor_;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_TOOL_SIMULATION_CLOCK_EXECUTOR_H_
