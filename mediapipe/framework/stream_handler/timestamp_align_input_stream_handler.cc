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

#include "mediapipe/framework/stream_handler/timestamp_align_input_stream_handler.h"

#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "absl/log/absl_check.h"
#include "absl/strings/substitute.h"
#include "absl/synchronization/mutex.h"
#include "mediapipe/framework/calculator_context_manager.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/collection_item_id.h"
#include "mediapipe/framework/input_stream_handler.h"
#include "mediapipe/framework/mediapipe_options.pb.h"
#include "mediapipe/framework/stream_handler/timestamp_align_input_stream_handler.pb.h"
#include "mediapipe/framework/timestamp.h"
#include "mediapipe/framework/tool/validate_name.h"

namespace mediapipe {

REGISTER_INPUT_STREAM_HANDLER(TimestampAlignInputStreamHandler);

TimestampAlignInputStreamHandler::TimestampAlignInputStreamHandler(
    std::shared_ptr<tool::TagMap> tag_map, CalculatorContextManager* cc_manager,
    const mediapipe::MediaPipeOptions& options, bool calculator_run_in_parallel)
    : InputStreamHandler(std::move(tag_map), cc_manager, options,
                         calculator_run_in_parallel),
      timestamp_offsets_(input_stream_managers_.NumEntries()) {
  const auto& handler_options = options.GetExtension(
      mediapipe::TimestampAlignInputStreamHandlerOptions::ext);
  std::string tag;
  int index;
  MEDIAPIPE_CHECK_OK(tool::ParseTagIndex(
      handler_options.timestamp_base_tag_index(), &tag, &index));
  timestamp_base_stream_id_ = input_stream_managers_.GetId(tag, index);
  ABSL_CHECK(timestamp_base_stream_id_.IsValid())
      << "stream \"" << handler_options.timestamp_base_tag_index()
      << "\" is not found.";
  timestamp_offsets_[timestamp_base_stream_id_.value()] = 0;
}

void TimestampAlignInputStreamHandler::PrepareForRun(
    std::function<void()> headers_ready_callback,
    std::function<void()> notification_callback,
    std::function<void(CalculatorContext*)> schedule_callback,
    std::function<void(absl::Status)> error_callback) {
  {
    absl::MutexLock lock(&mutex_);
    offsets_initialized_ = (input_stream_managers_.NumEntries() == 1);
  }

  InputStreamHandler::PrepareForRun(
      std::move(headers_ready_callback), std::move(notification_callback),
      std::move(schedule_callback), std::move(error_callback));
}

NodeReadiness TimestampAlignInputStreamHandler::GetNodeReadiness(
    Timestamp* min_stream_timestamp) {
  ABSL_DCHECK(min_stream_timestamp);
  *min_stream_timestamp = Timestamp::Done();
  Timestamp min_bound = Timestamp::Done();

  {
    absl::MutexLock lock(&mutex_);
    if (!offsets_initialized_) {
      bool timestamp_base_empty;
      *min_stream_timestamp =
          input_stream_managers_.Get(timestamp_base_stream_id_)
              ->MinTimestampOrBound(&timestamp_base_empty);
      if (timestamp_base_empty) {
        return NodeReadiness::kNotReady;
      }
      int unknown_non_base_stream_count = 0;
      for (CollectionItemId id = input_stream_managers_.BeginId();
           id < input_stream_managers_.EndId(); ++id) {
        if (id == timestamp_base_stream_id_) {
          continue;
        }
        const auto& stream = input_stream_managers_.Get(id);
        bool empty;
        Timestamp stream_timestamp = stream->MinTimestampOrBound(&empty);
        if (empty) {
          ++unknown_non_base_stream_count;
        } else {
          timestamp_offsets_[id.value()] =
              *min_stream_timestamp - stream_timestamp;
        }
      }
      if (unknown_non_base_stream_count == 0) {
        offsets_initialized_ = true;
      }
      return NodeReadiness::kReadyForProcess;
    }
  }

  for (CollectionItemId id = input_stream_managers_.BeginId();
       id < input_stream_managers_.EndId(); ++id) {
    const auto& stream = input_stream_managers_.Get(id);
    bool empty;
    Timestamp stream_timestamp = stream->MinTimestampOrBound(&empty);
    if (stream_timestamp.IsRangeValue()) {
      stream_timestamp += timestamp_offsets_[id.value()];
    }
    if (empty) {
      min_bound = std::min(min_bound, stream_timestamp);
    }
    *min_stream_timestamp = std::min(*min_stream_timestamp, stream_timestamp);
  }

  if (*min_stream_timestamp == Timestamp::Done()) {
    return NodeReadiness::kReadyForClose;
  }

  if (min_bound > *min_stream_timestamp) {
    return NodeReadiness::kReadyForProcess;
  }

  ABSL_CHECK_EQ(min_bound, *min_stream_timestamp);
  return NodeReadiness::kNotReady;
}

void TimestampAlignInputStreamHandler::FillInputSet(
    Timestamp input_timestamp, InputStreamShardSet* input_set) {
  ABSL_CHECK(input_timestamp.IsAllowedInStream());
  ABSL_CHECK(input_set);
  {
    absl::MutexLock lock(&mutex_);
    if (!offsets_initialized_) {
      for (CollectionItemId id = input_stream_managers_.BeginId();
           id < input_stream_managers_.EndId(); ++id) {
        const auto& stream = input_stream_managers_.Get(id);
        int num_packets_dropped = 0;
        bool stream_is_done = false;
        Packet current_packet;
        if (id == timestamp_base_stream_id_) {
          current_packet = stream->PopPacketAtTimestamp(
              input_timestamp, &num_packets_dropped, &stream_is_done);
          ABSL_CHECK_EQ(num_packets_dropped, 0) << absl::Substitute(
              "Dropped $0 packet(s) on input stream \"$1\".",
              num_packets_dropped, stream->Name());
        }
        AddPacketToShard(&input_set->Get(id), std::move(current_packet),
                         stream_is_done);
      }
      return;
    }
  }
  for (CollectionItemId id = input_stream_managers_.BeginId();
       id < input_stream_managers_.EndId(); ++id) {
    auto& stream = input_stream_managers_.Get(id);
    int num_packets_dropped = 0;
    bool stream_is_done = false;
    Timestamp stream_timestamp =
        input_timestamp - timestamp_offsets_[id.value()];
    Packet current_packet = stream->PopPacketAtTimestamp(
        stream_timestamp, &num_packets_dropped, &stream_is_done);
    if (!current_packet.IsEmpty()) {
      ABSL_CHECK_EQ(current_packet.Timestamp(), stream_timestamp);
      current_packet = current_packet.At(input_timestamp);
    }
    ABSL_CHECK_EQ(num_packets_dropped, 0)
        << absl::Substitute("Dropped $0 packet(s) on input stream \"$1\".",
                            num_packets_dropped, stream->Name());
    AddPacketToShard(&input_set->Get(id), std::move(current_packet),
                     stream_is_done);
  }
}

}  // namespace mediapipe
