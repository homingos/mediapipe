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

#include "mediapipe/framework/tool/name_util.h"

#include <set>
#include <unordered_map>

#include "absl/container/flat_hash_set.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_format.h"
#include "mediapipe/framework/port/map_util.h"
#include "mediapipe/framework/tool/validate_name.h"

namespace mediapipe {
namespace tool {

std::string GetUnusedNodeName(const CalculatorGraphConfig& config,
                              const std::string& node_name_base) {
  std::set<std::string> node_names;
  for (const ::mediapipe::CalculatorGraphConfig::Node& node : config.node()) {
    if (!node.name().empty()) {
      node_names.insert(node.name());
    }
  }

  std::string candidate = node_name_base;
  int iter = 2;
  while (ContainsKey(node_names, candidate)) {
    candidate =
        absl::StrCat(node_name_base, "_", absl::StrFormat("%02d", iter));
    ++iter;
  }
  return candidate;
}

std::string GetUnusedSidePacketName(
    const CalculatorGraphConfig& config,
    const std::string& input_side_packet_name_base) {
  absl::flat_hash_set<std::string> input_side_packets;
  for (const ::mediapipe::CalculatorGraphConfig::Node& node : config.node()) {
    for (const auto& tag_and_name : node.input_side_packet()) {
      std::string tag;
      std::string name;
      int index;
      MEDIAPIPE_CHECK_OK(ParseTagIndexName(tag_and_name, &tag, &index, &name));
      input_side_packets.insert(name);
    }
  }
  std::string candidate = input_side_packet_name_base;
  int iter = 2;
  while (input_side_packets.contains(candidate)) {
    candidate = absl::StrCat(input_side_packet_name_base, "_",
                             absl::StrFormat("%02d", iter));
    ++iter;
  }
  return candidate;
}

std::string CanonicalNodeName(const CalculatorGraphConfig& graph_config,
                              int node_id) {
  const auto& node_config = graph_config.node(node_id);
  std::string node_name = node_config.name().empty() ? node_config.calculator()
                                                     : node_config.name();
  int count = 0;
  int sequence = 0;
  for (int i = 0; i < graph_config.node_size(); i++) {
    const auto& current_node_config = graph_config.node(i);
    std::string current_node_name = current_node_config.name().empty()
                                        ? current_node_config.calculator()
                                        : current_node_config.name();
    if (node_name == current_node_name) {
      ++count;
      if (i < node_id) {
        ++sequence;
      }
    }
  }
  if (count <= 1) {
    return node_name;
  }
  return absl::StrCat(node_name, "_", sequence + 1);
}

std::string ParseNameFromStream(const std::string& stream) {
  std::string tag, name;
  int index;
  MEDIAPIPE_CHECK_OK(tool::ParseTagIndexName(stream, &tag, &index, &name));
  return name;
}

std::pair<std::string, int> ParseTagIndex(const std::string& tag_index) {
  std::string tag;
  int index;
  MEDIAPIPE_CHECK_OK(tool::ParseTagIndex(tag_index, &tag, &index));
  return {tag, index};
}

std::pair<std::string, int> ParseTagIndexFromStream(const std::string& stream) {
  std::string tag, name;
  int index;
  MEDIAPIPE_CHECK_OK(tool::ParseTagIndexName(stream, &tag, &index, &name));
  return {tag, index};
}

std::string CatTag(const std::string& tag, int index) {
  std::string colon_index =
      (index <= 0 || tag.empty()) ? "" : absl::StrCat(":", index);
  return absl::StrCat(tag, colon_index);
}

std::string CatStream(const std::pair<std::string, int>& tag_index,
                      const std::string& name) {
  std::string tag = CatTag(tag_index.first, tag_index.second);
  tag = tag.empty() ? tag : absl::StrCat(tag, ":");
  return absl::StrCat(tag, name);
}

}  // namespace tool
}  // namespace mediapipe
