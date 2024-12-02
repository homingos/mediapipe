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

#include "mediapipe/framework/stream_handler/default_input_stream_handler.h"

#include <algorithm>

#include "absl/strings/substitute.h"
#include "mediapipe/framework/input_stream_handler.h"

namespace mediapipe {

REGISTER_INPUT_STREAM_HANDLER(DefaultInputStreamHandler);

// Returns all CollectionItemId's for a Collection TagMap.
std::vector<CollectionItemId> GetIds(
    const std::shared_ptr<tool::TagMap>& tag_map) {
  std::vector<CollectionItemId> result;
  for (auto id = tag_map->BeginId(); id < tag_map->EndId(); ++id) {
    result.push_back(id);
  }
  return result;
}

DefaultInputStreamHandler::DefaultInputStreamHandler(
    std::shared_ptr<tool::TagMap> tag_map, CalculatorContextManager* cc_manager,
    const MediaPipeOptions& options, bool calculator_run_in_parallel)
    : InputStreamHandler(std::move(tag_map), cc_manager, options,
                         calculator_run_in_parallel),
      sync_set_(this, GetIds(input_stream_managers_.TagMap())) {
  if (options.HasExtension(DefaultInputStreamHandlerOptions::ext)) {
    SetBatchSize(options.GetExtension(DefaultInputStreamHandlerOptions::ext)
                     .batch_size());
  }
}

void DefaultInputStreamHandler::PrepareForRun(
    std::function<void()> headers_ready_callback,
    std::function<void()> notification_callback,
    std::function<void(CalculatorContext*)> schedule_callback,
    std::function<void(absl::Status)> error_callback) {
  sync_set_.PrepareForRun();
  InputStreamHandler::PrepareForRun(
      std::move(headers_ready_callback), std::move(notification_callback),
      std::move(schedule_callback), std::move(error_callback));
}

NodeReadiness DefaultInputStreamHandler::GetNodeReadiness(
    Timestamp* min_stream_timestamp) {
  return sync_set_.GetReadiness(min_stream_timestamp);
}

void DefaultInputStreamHandler::FillInputSet(Timestamp input_timestamp,
                                             InputStreamShardSet* input_set) {
  sync_set_.FillInputSet(input_timestamp, input_set);
}

}  // namespace mediapipe
