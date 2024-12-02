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
//
// This template is used by the mediapipe_simple_subgraph macro in
// //mediapipe/framework/tool/mediapipe_graph.bzl

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/subgraph.h"

namespace mediapipe {

// clang-format off
static const char binary_graph[] =
#include "{{SUBGRAPH_INC_FILE_PATH}}"
    ;  // NOLINT(whitespace/semicolon)

class {{SUBGRAPH_CLASS_NAME}} : public Subgraph {
 public:
  absl::StatusOr<CalculatorGraphConfig> GetConfig(
        const SubgraphOptions& /*options*/) {
    CalculatorGraphConfig config;
    // Note: this is a binary protobuf serialization, and may include NUL
    // bytes. The trailing NUL added to the string literal should be excluded.
    if (config.ParseFromArray(binary_graph, sizeof(binary_graph) - 1)) {
      return config;
    } else {
      return absl::InternalError("Could not parse subgraph.");
    }
  }
};
REGISTER_MEDIAPIPE_GRAPH({{SUBGRAPH_CLASS_NAME}});
// clang-format on

}  // namespace mediapipe
