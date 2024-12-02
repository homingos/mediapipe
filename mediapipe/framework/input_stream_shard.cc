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

#include "mediapipe/framework/input_stream_shard.h"

#include "absl/log/absl_check.h"

namespace mediapipe {

void InputStreamShard::AddPacket(Packet&& value, bool is_done) {
  // A packet can be added if the shard is still active or the packet being
  // added is empty. An empty packet corresponds to absence of a packet.
  ABSL_CHECK(!is_done_ || value.IsEmpty());
  packet_queue_.emplace(std::move(value));
  is_done_ = is_done;
}

}  // namespace mediapipe
