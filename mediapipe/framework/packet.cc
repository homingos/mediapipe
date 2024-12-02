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

#include "mediapipe/framework/packet.h"

#include "absl/log/absl_check.h"
#include "absl/strings/str_cat.h"
#include "mediapipe/framework/port.h"
#include "mediapipe/framework/port/canonical_errors.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/port/status_builder.h"
#include "mediapipe/framework/tool/type_util.h"

namespace mediapipe {
namespace packet_internal {

HolderBase::~HolderBase() {}

Packet Create(HolderBase* holder) {
  Packet result;
  result.holder_.reset(holder);
  return result;
}

Packet Create(HolderBase* holder, Timestamp timestamp) {
  Packet result;
  result.holder_.reset(holder);
  result.timestamp_ = timestamp;
  return result;
}

Packet Create(std::shared_ptr<HolderBase> holder, Timestamp timestamp) {
  Packet result;
  result.holder_ = std::move(holder);
  result.timestamp_ = timestamp;
  return result;
}

const HolderBase* GetHolder(const Packet& packet) {
  return packet.holder_.get();
}

absl::StatusOr<Packet> PacketFromDynamicProto(const std::string& type_name,
                                              const std::string& serialized) {
  MP_ASSIGN_OR_RETURN(
      auto message_holder,
      packet_internal::MessageHolderRegistry::CreateByName(type_name));
  auto* message =
      const_cast<proto_ns::MessageLite*>(message_holder->GetProtoMessageLite());
  RET_CHECK_NE(message, nullptr);
  RET_CHECK(message->ParseFromString(serialized));
  return packet_internal::Create(message_holder.release());
}

}  // namespace packet_internal

Packet Packet::At(class Timestamp timestamp) const& {
  Packet result(*this);
  result.timestamp_ = timestamp;
  return result;
}

Packet Packet::At(class Timestamp timestamp) && {
  timestamp_ = timestamp;
  // Note: Below object is of type Packet&& which doesn't match return type
  // of Packet, so a new Packet will be created by move constructor here.
  // Moving everything to a new Packet is done intentionally to preserve the
  // immutability of Timestamps.
  return std::move(*this);
}

std::string Packet::RegisteredTypeName() const {
  if (IsEmpty()) {
    return "";
  }
  return holder_->RegisteredTypeName();
}

std::string Packet::DebugTypeName() const {
  if (IsEmpty()) {
    return "{empty}";
  }
  return holder_->DebugTypeName();
}

std::string Packet::DebugString() const {
  std::string result = absl::StrCat("mediapipe::Packet with timestamp: ",
                                    timestamp_.DebugString());
  if (IsEmpty()) {
    absl::StrAppend(&result, " and no data");
  } else {
    absl::StrAppend(&result, " and type: ", holder_->DebugTypeName());
  }
  return result;
}

absl::Status Packet::ValidateAsType(TypeId type_id) const {
  if (ABSL_PREDICT_FALSE(IsEmpty())) {
    return absl::InternalError(absl::StrCat(
        "Expected a Packet of type: ", MediaPipeTypeStringOrDemangled(type_id),
        ", but received an empty Packet."));
  }
  bool holder_is_right_type = holder_->GetTypeId() == type_id;
  if (ABSL_PREDICT_FALSE(!holder_is_right_type)) {
    return absl::InvalidArgumentError(absl::StrCat(
        "The Packet stores \"", holder_->DebugTypeName(), "\", but \"",
        MediaPipeTypeStringOrDemangled(type_id), "\" was requested."));
  }
  return absl::OkStatus();
}

absl::Status Packet::ValidateAsProtoMessageLite() const {
  if (ABSL_PREDICT_FALSE(IsEmpty())) {
    return absl::InternalError("Packet is empty.");
  }
  if (ABSL_PREDICT_FALSE(holder_->GetProtoMessageLite() == nullptr)) {
    return absl::InvalidArgumentError(
        absl::StrCat("The Packet stores \"", holder_->DebugTypeName(), "\"",
                     "which is not convertible to proto_ns::MessageLite."));
  } else {
    return absl::OkStatus();
  }
}

const proto_ns::MessageLite& Packet::GetProtoMessageLite() const {
  ABSL_CHECK(holder_ != nullptr) << "The packet is empty.";
  const proto_ns::MessageLite* proto = holder_->GetProtoMessageLite();
  ABSL_CHECK(proto != nullptr)
      << "The Packet stores '" << holder_->DebugTypeName()
      << "', it cannot be converted to MessageLite type.";
  return *proto;
}

StatusOr<std::vector<const proto_ns::MessageLite*>>
Packet::GetVectorOfProtoMessageLitePtrs() const {
  if (holder_ == nullptr) {
    return absl::InternalError("Packet is empty.");
  }
  return holder_->GetVectorOfProtoMessageLite();
}

MEDIAPIPE_REGISTER_TYPE(::mediapipe::Packet, "::mediapipe::Packet", nullptr,
                        nullptr);
MEDIAPIPE_REGISTER_TYPE(::std::vector<::mediapipe::Packet>,
                        "::std::vector<::mediapipe::Packet>", nullptr, nullptr);
MEDIAPIPE_REGISTER_TYPE(::mediapipe::Timestamp, "::mediapipe::Timestamp",
                        nullptr, nullptr);
#define PACKET_PAIR_TYPE ::std::pair<::mediapipe::Packet, ::mediapipe::Packet>
MEDIAPIPE_REGISTER_TYPE(PACKET_PAIR_TYPE,
                        "::std::pair<::mediapipe::Packet,::mediapipe::Packet>",
                        nullptr, nullptr);
#undef PACKET_PAIR_TYPE
#define PACKET_MAP_TYPE ::std::map<std::string, ::mediapipe::Packet>
MEDIAPIPE_REGISTER_TYPE(PACKET_MAP_TYPE,
                        "::std::map<std::string,::mediapipe::Packet>", nullptr,
                        nullptr);
#undef PACKET_MAP_TYPE

}  // namespace mediapipe
