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

#include "mediapipe/framework/output_side_packet_impl.h"

#include "absl/log/absl_check.h"
#include "mediapipe/framework/port/logging.h"
#include "mediapipe/framework/port/source_location.h"
#include "mediapipe/framework/port/status_builder.h"

namespace mediapipe
{

  absl::Status OutputSidePacketImpl::Initialize(const std::string &name,
                                                const PacketType *packet_type)
  {
    name_ = name;
    packet_type_ = packet_type;
    return absl::OkStatus();
  }

  void OutputSidePacketImpl::PrepareForRun(
      std::function<void(absl::Status)> error_callback)
  {
    error_callback_ = std::move(error_callback);
    initialized_ = false;
  }

  void OutputSidePacketImpl::Set(const Packet &packet)
  {
    absl::Status status = SetInternal(packet);
    if (!status.ok())
    {
      TriggerErrorCallback(status);
    }
  }

  void OutputSidePacketImpl::AddMirror(
      InputSidePacketHandler *input_side_packet_handler, CollectionItemId id)
  {
    ABSL_CHECK(input_side_packet_handler);
    mirrors_.emplace_back(input_side_packet_handler, id);
  }

  absl::Status OutputSidePacketImpl::SetInternal(const Packet &packet)
  {
    // if (initialized_) {
    //   return mediapipe::AlreadyExistsErrorBuilder(MEDIAPIPE_LOC)
    //          << "Output side packet \"" << name_ << "\" was already set.";
    // }

    if (packet.IsEmpty())
    {
      return mediapipe::InvalidArgumentErrorBuilder(MEDIAPIPE_LOC)
             << "Empty packet set on output side packet \"" << name_ << "\".";
    }

    if (packet.Timestamp() != Timestamp::Unset())
    {
      return mediapipe::InvalidArgumentErrorBuilder(MEDIAPIPE_LOC)
             << "Output side packet \"" << name_ << "\" has a timestamp "
             << packet.Timestamp().DebugString() << ".";
    }

    absl::Status result = packet_type_->Validate(packet);
    if (!result.ok())
    {
      return mediapipe::StatusBuilder(result, MEDIAPIPE_LOC).SetPrepend()
             << absl::StrCat(
                    "Packet type mismatch on calculator output side packet \"",
                    name_, "\": ");
    }

    packet_ = packet;
    initialized_ = true;
    for (const auto &mirror : mirrors_)
    {
      mirror.input_side_packet_handler->Set(mirror.id, packet_);
    }
    return absl::OkStatus();
  }

  void OutputSidePacketImpl::TriggerErrorCallback(
      const absl::Status &status) const
  {
    ABSL_CHECK(error_callback_);
    error_callback_(status);
  }

} // namespace mediapipe
