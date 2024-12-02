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

#include "mediapipe/framework/tool/fill_packet_set.h"

#include <utility>
#include <vector>

#include "absl/memory/memory.h"
#include "mediapipe/framework/port/status_builder.h"
#include "mediapipe/framework/port/statusor.h"
#include "mediapipe/framework/tool/status_util.h"

namespace mediapipe {
namespace tool {

absl::StatusOr<std::unique_ptr<PacketSet>> FillPacketSet(
    const PacketTypeSet& input_side_packet_types,
    const std::map<std::string, Packet>& input_side_packets,
    int* missing_packet_count_ptr) {
  if (missing_packet_count_ptr != nullptr) {
    *missing_packet_count_ptr = 0;
  }
  std::vector<absl::Status> errors;
  auto packet_set =
      absl::make_unique<PacketSet>(input_side_packet_types.TagMap());
  const auto& names = input_side_packet_types.TagMap()->Names();
  for (CollectionItemId id = input_side_packet_types.BeginId();
       id < input_side_packet_types.EndId(); ++id) {
    const std::string& name = names[id.value()];
    const auto iter = input_side_packets.find(name);
    if (iter == input_side_packets.end()) {
      if (missing_packet_count_ptr != nullptr) {
        ++(*missing_packet_count_ptr);
      } else {
        errors.push_back(mediapipe::InvalidArgumentErrorBuilder(MEDIAPIPE_LOC)
                         << "Missing input side packet: " << name);
      }
      continue;
    }
    packet_set->Get(id) = iter->second;
    // Check the type.
    absl::Status status =
        input_side_packet_types.Get(id).Validate(iter->second);
    if (!status.ok()) {
      std::pair<std::string, int> tag_index =
          input_side_packet_types.TagAndIndexFromId(id);
      errors.push_back(
          mediapipe::StatusBuilder(status, MEDIAPIPE_LOC).SetPrepend()
          << "Packet \""
          << input_side_packet_types.TagMap()->Names()[id.value()]
          << "\" with tag \"" << tag_index.first << "\" and index "
          << tag_index.second << " failed validation.  ");
    }
  }
  if (!errors.empty()) {
    return tool::CombinedStatus("FillPacketSet failed:", errors);
  }
  return std::move(packet_set);
}

}  // namespace tool
}  // namespace mediapipe
