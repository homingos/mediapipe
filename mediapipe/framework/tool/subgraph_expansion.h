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

#ifndef MEDIAPIPE_FRAMEWORK_TOOL_SUBGRAPH_EXPANSION_H_
#define MEDIAPIPE_FRAMEWORK_TOOL_SUBGRAPH_EXPANSION_H_

#include <functional>

#include "absl/strings/string_view.h"
#include "mediapipe/framework/calculator.pb.h"
#include "mediapipe/framework/graph_service_manager.h"
#include "mediapipe/framework/port/proto_ns.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/subgraph.h"

namespace mediapipe {

namespace tool {

// Apply the given transformation function to the names of streams and
// side packets.
absl::Status TransformStreamNames(
    proto_ns::RepeatedPtrField<ProtoString>* streams,
    const std::function<std::string(absl::string_view)>& transform);

// Apply the given transformation function to the names of streams,
// side packets, and nodes.
absl::Status TransformNames(
    CalculatorGraphConfig* config,
    const std::function<std::string(absl::string_view)>& transform);

// Updates the given map with entries mapping the names of streams in the
// source set to those of the corresponding streams in the destination set.
// Corresponding streams are those with the same tag and index. Streams with
// no match are ignored.
//
// For instance, given:
//   src: FOO:abc    dst: FOO:bob
//        BAR:def
// The entry 'abc' -> 'bob' is added to the map.
absl::Status FindCorrespondingStreams(
    std::map<std::string, std::string>* stream_map,
    const proto_ns::RepeatedPtrField<ProtoString>& src_streams,
    const proto_ns::RepeatedPtrField<ProtoString>& dst_streams);

// Validates the fields in the given Node message that specifies a subgraph.
// Returns an error status if the Node message contains any field that is only
// applicable to calculators.
absl::Status ValidateSubgraphFields(
    const CalculatorGraphConfig::Node& subgraph_node);

// Renames the streams in a subgraph config to match the connections on the
// wrapping node.
absl::Status ConnectSubgraphStreams(
    const CalculatorGraphConfig::Node& subgraph_node,
    CalculatorGraphConfig* subgraph_config);

// Replaces subgraph nodes in the given config with the contents of the
// corresponding subgraphs. Nested subgraphs are retrieved from the
// graph registry and expanded recursively.
absl::Status ExpandSubgraphs(
    CalculatorGraphConfig* config,
    const GraphRegistry* graph_registry = nullptr,
    const Subgraph::SubgraphOptions* graph_options = nullptr,
    const GraphServiceManager* service_manager = nullptr);

// Creates a graph wrapping the provided node and exposing all of its
// connections
CalculatorGraphConfig MakeSingleNodeGraph(
    CalculatorGraphConfig::Node subgraph_node);

}  // namespace tool
}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_TOOL_SUBGRAPH_EXPANSION_H_
