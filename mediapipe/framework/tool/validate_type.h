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

// Helper functions for doing type validation on CalculatorGraphConfig.
#ifndef MEDIAPIPE_FRAMEWORK_TOOL_VALIDATE_TYPE_H_
#define MEDIAPIPE_FRAMEWORK_TOOL_VALIDATE_TYPE_H_

#include <map>

#include "mediapipe/framework/calculator.pb.h"
#include "mediapipe/framework/packet_set.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {

namespace tool {
// Equivalent functions for PacketGenerators.
absl::Status RunGeneratorFillExpectations(
    const PacketGeneratorConfig& config,
    const std::string& package = "mediapipe");

// Run PacketGenerator::Generate() on the given generator, options,
// and inputs to produce outputs.  Validate the types of the inputs and
// outputs using PacketGenerator::FillExpectations.
absl::Status RunGenerateAndValidateTypes(
    const std::string& packet_generator_name,
    const PacketGeneratorOptions& extendable_options,
    const PacketSet& input_side_packets, PacketSet* output_side_packets,
    const std::string& package = "mediapipe");

}  // namespace tool
}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_TOOL_VALIDATE_TYPE_H_
