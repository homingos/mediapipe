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

#ifndef MEDIAPIPE_FRAMEWORK_STREAM_HANDLER_IN_ORDER_OUTPUT_STREAM_HANDLER_H_
#define MEDIAPIPE_FRAMEWORK_STREAM_HANDLER_IN_ORDER_OUTPUT_STREAM_HANDLER_H_

#include <memory>
#include <utility>

// TODO: Move protos in another CL after the C++ code migration.
#include "mediapipe/framework/mediapipe_options.pb.h"
#include "mediapipe/framework/output_stream_handler.h"
#include "mediapipe/framework/packet_set.h"
#include "mediapipe/framework/timestamp.h"
#include "mediapipe/framework/tool/tag_map.h"

namespace mediapipe {

// InOrderOutputStreamHandler supports both sequential and parallel processing
// of input packets, and will deliver the output packets in increasing timestamp
// order.
class InOrderOutputStreamHandler : public OutputStreamHandler {
 public:
  InOrderOutputStreamHandler(
      std::shared_ptr<tool::TagMap> tag_map,
      CalculatorContextManager* calculator_context_manager,
      const MediaPipeOptions& options, bool calculator_run_in_parallel)
      : OutputStreamHandler(std::move(tag_map), calculator_context_manager,
                            options, calculator_run_in_parallel) {}

 private:
  void PropagationLoop() ABSL_EXCLUSIVE_LOCKS_REQUIRED(timestamp_mutex_) final;

  void PropagatePackets(CalculatorContext** calculator_context,
                        Timestamp* context_timestamp)
      ABSL_EXCLUSIVE_LOCKS_REQUIRED(timestamp_mutex_);

  void PropagationBound(CalculatorContext** calculator_context,
                        Timestamp* context_timestamp)
      ABSL_EXCLUSIVE_LOCKS_REQUIRED(timestamp_mutex_);
};
}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_STREAM_HANDLER_IN_ORDER_OUTPUT_STREAM_HANDLER_H_
