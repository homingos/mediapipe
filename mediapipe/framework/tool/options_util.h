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

#ifndef MEDIAPIPE_FRAMEWORK_TOOL_OPTIONS_UTIL_H_
#define MEDIAPIPE_FRAMEWORK_TOOL_OPTIONS_UTIL_H_

#include "mediapipe/framework/calculator.pb.h"
#include "mediapipe/framework/input_stream_shard.h"
#include "mediapipe/framework/packet.h"
#include "mediapipe/framework/packet_set.h"
#include "mediapipe/framework/port/any_proto.h"
#include "mediapipe/framework/tool/options_map.h"

namespace mediapipe {

namespace tool {

// Combine a base options with an overriding options.
template <typename T>
inline T MergeOptions(const T& base, const T& options) {
  auto result = base;
  result.MergeFrom(options);
  return result;
}

// Combine a base options message with an optional side packet. The specified
// packet can hold either the specified options type T or CalculatorOptions.
// Fields are either replaced or merged depending on field merge_fields.
template <typename T>
inline T RetrieveOptions(const T& base, const Packet& options_packet) {
  if (!options_packet.IsEmpty()) {
    T packet_options;
    if (options_packet.ValidateAsType<T>().ok()) {
      packet_options = options_packet.Get<T>();
    } else if (options_packet.ValidateAsType<CalculatorOptions>().ok()) {
      GetExtension<T>(options_packet.Get<CalculatorOptions>(), &packet_options);
    }
    return tool::MergeOptions(base, packet_options);
  }
  return base;
}

// Combine a base options message with an optional side packet from
// a PacketSet such as a calculator's input-side-packets.
template <typename T>
inline T RetrieveOptions(const T& base, const PacketSet& packet_set,
                         const std::string& tag_name = "OPTIONS") {
  if (packet_set.HasTag(tag_name)) {
    return tool::RetrieveOptions(base, packet_set.Tag(tag_name));
  }
  return base;
}

// Combine a base options message with an optional input packet from
// an InputStreamShardSet such as a calculator's input streams.
template <typename T>
inline T RetrieveOptions(const T& base, const InputStreamShardSet& stream_set,
                         const std::string& tag_name = "OPTIONS") {
  if (stream_set.HasTag(tag_name)) {
    Packet options_packet = stream_set.Tag(tag_name).Value();
    return tool::RetrieveOptions(base, options_packet);
  }
  return base;
}

// Copy literal options from enclosing graphs.
absl::Status DefineGraphOptions(const CalculatorGraphConfig::Node& parent_node,
                                CalculatorGraphConfig* config);

}  // namespace tool
}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_TOOL_OPTIONS_UTIL_H_
