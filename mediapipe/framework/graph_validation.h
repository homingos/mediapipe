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
// Declares CalculatorGraph, which links Calculators into a directed acyclic
// graph, and allows its evaluation.

#ifndef MEDIAPIPE_FRAMEWORK_GRAPH_VALIDATION_H_
#define MEDIAPIPE_FRAMEWORK_GRAPH_VALIDATION_H_

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {

// Validates a CalculatorGraphConfig, including subgraphs, template graphs,
// and side-packets.
class GraphValidation {
 public:
  // Validates the specified CalculatorGraphConfig.
  absl::Status Validate(
      const CalculatorGraphConfig& config,
      const std::map<std::string, Packet>& side_packets = {}) {
    return graph_.Initialize(config, side_packets);
  }

  // Validates the specified CalculatorGraphConfigs.
  // Template graph and subgraph configs can be specified through
  // |input_templates|.  Every subgraph must have its graph type specified in
  // CalclatorGraphConfig.type.  A subgraph can be validated directly by
  // specifying its type in |graph_type|.  A template graph can be validated
  // directly by specifying its template arguments in |arguments|.
  absl::Status Validate(const std::vector<CalculatorGraphConfig>& configs,
                        const std::vector<CalculatorGraphTemplate>& templates,
                        const std::map<std::string, Packet>& side_packets = {},
                        const std::string& graph_type = "",
                        const Subgraph::SubgraphOptions* options = nullptr) {
    return graph_.Initialize(configs, templates, side_packets, graph_type,
                             options);
  }

 private:
  CalculatorGraph graph_;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_GRAPH_VALIDATION_H_
