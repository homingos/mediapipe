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

#ifndef MEDIAPIPE_FRAMEWORK_INPUT_SIDE_PACKET_HANDLER_H_
#define MEDIAPIPE_FRAMEWORK_INPUT_SIDE_PACKET_HANDLER_H_

#include <atomic>
#include <functional>
#include <map>
#include <memory>
#include <string>

#include "mediapipe/framework/collection_item_id.h"
#include "mediapipe/framework/packet.h"
#include "mediapipe/framework/packet_set.h"
#include "mediapipe/framework/packet_type.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {

// The input side packet handler is invoked every time any of the input side
// packets is set and notifies the calculator node when all the input side
// packets become available. The input side packet handler owns and manages
// the input side packets. OutputSidePacket sets an input side packet through
// its input side packet handler.
class InputSidePacketHandler {
 public:
  InputSidePacketHandler() = default;
  ~InputSidePacketHandler() = default;

  // Resets the input side packet handler and its underlying input side packets
  // for another run of the graph.
  absl::Status PrepareForRun(
      const PacketTypeSet* input_side_packet_types,
      const std::map<std::string, Packet>& all_side_packets,
      std::function<void()> input_side_packets_ready_callback,
      std::function<void(absl::Status)> error_callback);

  // Sets a particular input side packet.
  void Set(CollectionItemId id, const Packet& packet);

  const PacketSet& InputSidePackets() const { return *input_side_packets_; }

  // Returns true if the set of input-side-packets has changed since the
  // previous run.
  bool InputSidePacketsChanged();

  // Returns the number of missing input side packets.
  int MissingInputSidePacketCount() const {
    return missing_input_side_packet_count_.load(std::memory_order_relaxed);
  }

 private:
  // Called by Set().
  absl::Status SetInternal(CollectionItemId id, const Packet& packet);

  // Triggers the error callback with absl::Status info when an error
  // occurs.
  void TriggerErrorCallback(const absl::Status& status) const;

  const PacketTypeSet* input_side_packet_types_;

  std::unique_ptr<PacketSet> input_side_packets_;
  std::unique_ptr<PacketSet> prev_input_side_packets_;

  std::atomic<int> missing_input_side_packet_count_{0};

  std::function<void()> input_side_packets_ready_callback_;
  std::function<void(absl::Status)> error_callback_;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_INPUT_SIDE_PACKET_HANDLER_H_
