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

#ifndef MEDIAPIPE_FRAMEWORK_TOOL_FILL_PACKET_SET_H_
#define MEDIAPIPE_FRAMEWORK_TOOL_FILL_PACKET_SET_H_

#include <map>
#include <memory>
#include <string>

#include "mediapipe/framework/packet_set.h"
#include "mediapipe/framework/packet_type.h"
#include "mediapipe/framework/port/statusor.h"

namespace mediapipe {
namespace tool {

// Finds the packet names from input_side_packet_types's TagMap, looks
// them up in |input_side_packets| and creates a PacketSet.  An error
// is returned if any packets fail the type check.  If
// missing_packet_count_ptr is not null, the number of missing packets
// is returned in *missing_packet_count_ptr.  Otherwise, an error is
// returned if any packets are missing.
absl::StatusOr<std::unique_ptr<PacketSet>> FillPacketSet(
    const PacketTypeSet& input_side_packet_types,
    const std::map<std::string, Packet>& input_side_packets,
    int* missing_packet_count_ptr);

}  // namespace tool
}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_TOOL_FILL_PACKET_SET_H_
