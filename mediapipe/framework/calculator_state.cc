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

// Definitions for CalculatorNode.

#include "mediapipe/framework/calculator_state.h"

#include <string>

#include "absl/log/absl_check.h"
#include "absl/strings/str_cat.h"
#include "mediapipe/framework/port/logging.h"

namespace mediapipe {

CalculatorState::CalculatorState(
    const std::string& node_name, int node_id,
    const std::string& calculator_type,
    const CalculatorGraphConfig::Node& node_config,
    std::shared_ptr<ProfilingContext> profiling_context)
    : node_name_(node_name),
      node_id_(node_id),
      calculator_type_(calculator_type),
      node_config_(node_config),
      profiling_context_(profiling_context),
      counter_factory_(nullptr) {
  options_.Initialize(node_config);
  ResetBetweenRuns();
}

CalculatorState::~CalculatorState() {}

void CalculatorState::ResetBetweenRuns() {
  input_side_packets_ = nullptr;
  counter_factory_ = nullptr;
}

void CalculatorState::SetInputSidePackets(const PacketSet* input_side_packets) {
  ABSL_CHECK(input_side_packets);
  input_side_packets_ = input_side_packets;
}

void CalculatorState::SetOutputSidePackets(
    OutputSidePacketSet* output_side_packets) {
  ABSL_CHECK(output_side_packets);
  output_side_packets_ = output_side_packets;
}

Counter* CalculatorState::GetCounter(const std::string& name) {
  ABSL_CHECK(counter_factory_);
  return counter_factory_->GetCounter(absl::StrCat(NodeName(), "-", name));
}

CounterFactory* CalculatorState::GetCounterFactory() {
  ABSL_CHECK(counter_factory_);
  return counter_factory_;
}

}  // namespace mediapipe
