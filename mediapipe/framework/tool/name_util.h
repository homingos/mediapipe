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

#ifndef MEDIAPIPE_FRAMEWORK_TOOL_NAME_UTIL_H_
#define MEDIAPIPE_FRAMEWORK_TOOL_NAME_UTIL_H_

#include <string>

#include "mediapipe/framework/calculator.pb.h"

namespace mediapipe {

namespace tool {
// Get an unused InputSidePacket name which is (or starts with)
// input_side_packet_name_base.
std::string GetUnusedSidePacketName(const CalculatorGraphConfig& /*config*/,
                                    const std::string& side_packet_name_base);

// Get an usused node name which is (or starts with) node_name_base.
std::string GetUnusedNodeName(const CalculatorGraphConfig& config,
                              const std::string& node_name_base);

// Returns a short unique name for a Node in a CalculatorGraphConfig.
// This is the Node.name (if specified) or the Node.calculator.
// If there are multiple calculators with similar name in the graph, the name
// will be postfixed by "_<COUNT>". For example, in the following graph the node
// names will be as mentiond.
//
// node { // Name will be "CalcA"
//   calculator: "CalcA"
// }
// node { // Name will be "NameB"
//   calculator: "CalcB"
//   name: "NameB"
// }
// node { // Name will be "CalcC_1" due to duplicate "calculator" field.
//   calculator: "CalcC"
// }
// node { // Name will be "CalcC_2" due to duplicate "calculator" field.
//   calculator: "CalcC"
// }
// node { // Name will be "NameX".
//   calculator: "CalcD"
//   name: "NameX"
// }
// node { // Name will be "NameY".
//   calculator: "CalcD"
//   name: "NameY"
// }
// node { // Name will be "NameZ_1". due to "name" field duplicate.
//   calculator: "CalcE"
//   name: "NameZ"
// }
// node { // Name will be "NameZ_2". due to "name" field duplicate.
//   calculator: "CalcF"
//   name: "NameZ"
// }
//
// TODO: Update GraphNode.UniqueName in MediaPipe Visualizer to match
// this logic.
// TODO: Fix the edge case mentioned in the bug.
std::string CanonicalNodeName(const CalculatorGraphConfig& graph_config,
                              int node_id);

// Parses the name from a "tag:index:name".
std::string ParseNameFromStream(const std::string& stream);

// Parses the TagIndex from a "tag:index".
std::pair<std::string, int> ParseTagIndex(const std::string& tag_index);

// Parses the TagIndex from a "tag:index:name".
std::pair<std::string, int> ParseTagIndexFromStream(const std::string& stream);

// Formats to "tag:index".
std::string CatTag(const std::string& tag, int index);

// Concatenates "tag:index:name" into a single string.
std::string CatStream(const std::pair<std::string, int>& tag_index,
                      const std::string& name);

}  // namespace tool
}  // namespace mediapipe

namespace mediapipe {
using mediapipe::tool::CanonicalNodeName;
}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_TOOL_NAME_UTIL_H_
