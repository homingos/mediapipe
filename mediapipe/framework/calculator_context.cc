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

#include "mediapipe/framework/calculator_context.h"

#include "absl/log/absl_check.h"

namespace mediapipe {

const std::string& CalculatorContext::CalculatorType() const {
  ABSL_CHECK(calculator_state_);
  return calculator_state_->CalculatorType();
}

const CalculatorOptions& CalculatorContext::Options() const {
  ABSL_CHECK(calculator_state_);
  return calculator_state_->Options();
}

const std::string& CalculatorContext::NodeName() const {
  ABSL_CHECK(calculator_state_);
  return calculator_state_->NodeName();
}

int CalculatorContext::NodeId() const {
  ABSL_CHECK(calculator_state_);
  return calculator_state_->NodeId();
}

Counter* CalculatorContext::GetCounter(const std::string& name) {
  ABSL_CHECK(calculator_state_);
  return calculator_state_->GetCounter(name);
}

CounterFactory* CalculatorContext::GetCounterFactory() {
  ABSL_CHECK(calculator_state_);
  return calculator_state_->GetCounterFactory();
}

const PacketSet& CalculatorContext::InputSidePackets() const {
  return calculator_state_->InputSidePackets();
}

OutputSidePacketSet& CalculatorContext::OutputSidePackets() {
  return calculator_state_->OutputSidePackets();
}

InputStreamShardSet& CalculatorContext::Inputs() { return inputs_; }

const InputStreamShardSet& CalculatorContext::Inputs() const { return inputs_; }

OutputStreamShardSet& CalculatorContext::Outputs() { return outputs_; }

const OutputStreamShardSet& CalculatorContext::Outputs() const {
  return outputs_;
}

void CalculatorContext::SetOffset(TimestampDiff offset) {
  for (auto& stream : outputs_) {
    stream.SetOffset(offset);
  }
}

const InputStreamSet& CalculatorContext::InputStreams() const {
  if (!input_streams_) {
    input_streams_ = absl::make_unique<InputStreamSet>(inputs_.TagMap());
    for (CollectionItemId id = input_streams_->BeginId();
         id < input_streams_->EndId(); ++id) {
      input_streams_->Get(id) = const_cast<InputStreamShard*>(&inputs_.Get(id));
    }
  }
  return *input_streams_;
}

const OutputStreamSet& CalculatorContext::OutputStreams() const {
  if (!output_streams_) {
    output_streams_ = absl::make_unique<OutputStreamSet>(outputs_.TagMap());
    for (CollectionItemId id = output_streams_->BeginId();
         id < output_streams_->EndId(); ++id) {
      output_streams_->Get(id) =
          const_cast<OutputStreamShard*>(&outputs_.Get(id));
    }
  }
  return *output_streams_;
}

}  // namespace mediapipe
