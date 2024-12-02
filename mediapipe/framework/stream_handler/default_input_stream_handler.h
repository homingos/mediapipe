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

#ifndef MEDIAPIPE_FRAMEWORK_STREAM_HANDLER_DEFAULT_INPUT_STREAM_HANDLER_H_
#define MEDIAPIPE_FRAMEWORK_STREAM_HANDLER_DEFAULT_INPUT_STREAM_HANDLER_H_

#include <memory>
#include <vector>

// TODO: Move protos in another CL after the C++ code migration.
#include "mediapipe/framework/input_stream_handler.h"
#include "mediapipe/framework/stream_handler/default_input_stream_handler.pb.h"

namespace mediapipe {

// Implementation of the "default" input stream handler that is applied on a
// given CalculatorGraph when no input stream handler is explicitly specified.
class DefaultInputStreamHandler : public InputStreamHandler {
 public:
  DefaultInputStreamHandler() = delete;
  DefaultInputStreamHandler(std::shared_ptr<tool::TagMap> tag_map,
                            CalculatorContextManager* cc_manager,
                            const MediaPipeOptions& options,
                            bool calculator_run_in_parallel);

 protected:
  // Reinitializes this InputStreamHandler before each CalculatorGraph run.
  void PrepareForRun(std::function<void()> headers_ready_callback,
                     std::function<void()> notification_callback,
                     std::function<void(CalculatorContext*)> schedule_callback,
                     std::function<void(absl::Status)> error_callback) override;

  // In DefaultInputStreamHandler, a node is "ready" if:
  // - all streams are done (need to call Close() in this case), or
  // - the minimum bound (over all empty streams) is greater than the smallest
  //   timestamp of any stream, which means we have received all the packets
  //   that will be available at the next timestamp.
  NodeReadiness GetNodeReadiness(Timestamp* min_stream_timestamp) override;

  // Only invoked when associated GetNodeReadiness() returned kReadyForProcess.
  void FillInputSet(Timestamp input_timestamp,
                    InputStreamShardSet* input_set) override;

  // The packet-set builder.
  SyncSet sync_set_;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_STREAM_HANDLER_DEFAULT_INPUT_STREAM_HANDLER_H_
