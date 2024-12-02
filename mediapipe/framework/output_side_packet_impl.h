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

#ifndef MEDIAPIPE_FRAMEWORK_OUTPUT_SIDE_PACKET_IMPL_H_
#define MEDIAPIPE_FRAMEWORK_OUTPUT_SIDE_PACKET_IMPL_H_

#include <functional>
#include <string>
#include <vector>

#include "mediapipe/framework/collection_item_id.h"
#include "mediapipe/framework/input_side_packet_handler.h"
#include "mediapipe/framework/output_side_packet.h"
#include "mediapipe/framework/packet.h"
#include "mediapipe/framework/packet_type.h"
#include "mediapipe/framework/port.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {

class OutputSidePacketImpl : public OutputSidePacket {
 public:
  OutputSidePacketImpl() = default;
  ~OutputSidePacketImpl() override = default;

  // Initializes the OutputSidePacketImpl.
  absl::Status Initialize(const std::string& name,
                          const PacketType* packet_type);

  // Prepares this for processing. If an error occurs in a user called function
  // (such as Set()) then error_callback will be called before returning
  // control to the user.
  void PrepareForRun(std::function<void(absl::Status)> error_callback);

  // Gets the output side packet.
  Packet GetPacket() const { return packet_; }

  // Sets the output side packet. The Packet must contain the data.
  //
  // NOTE: Set() cannot report errors via the return value. It uses an error
  // callback function to report errors.
  void Set(const Packet& packet) override;

  // Adds an input side packet, which is represented as a pointer to an
  // InputSidePacketHandler and a CollectionItemId, to mirrors_.
  // The caller retains the ownership of the InputSidePacketHandler.
  void AddMirror(InputSidePacketHandler* input_side_packet_handler,
                 CollectionItemId id);

 private:
  // The necessary information to locate an input side packet.
  struct Mirror {
    Mirror(InputSidePacketHandler* input_side_packet_handler,
           const CollectionItemId& id)
        : input_side_packet_handler(input_side_packet_handler), id(id) {}

    InputSidePacketHandler* const input_side_packet_handler;
    const CollectionItemId id;
  };

  // Called by Set().
  absl::Status SetInternal(const Packet& packet);

  // Triggers the error callback with absl::Status info when an error
  // occurs.
  void TriggerErrorCallback(const absl::Status& status) const;

  std::string name_;
  const PacketType* packet_type_;
  std::function<void(absl::Status)> error_callback_;
  Packet packet_;
  bool initialized_ = false;

  std::vector<Mirror> mirrors_;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_OUTPUT_SIDE_PACKET_IMPL_H_
