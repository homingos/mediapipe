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

#include "mediapipe/framework/calculator_contract.h"

#include <memory>
#include <utility>
#include <vector>

#include "absl/memory/memory.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/port/status_builder.h"
#include "mediapipe/framework/port/status_macros.h"
#include "mediapipe/framework/tool/packet_generator_wrapper_calculator.pb.h"
#include "mediapipe/framework/tool/tag_map.h"

namespace mediapipe {

namespace {

CalculatorGraphConfig::Node MakePacketGeneratorWrapperConfig(
    const PacketGeneratorConfig& node, const std::string& package) {
  CalculatorGraphConfig::Node wrapper_node;
  wrapper_node.set_calculator("PacketGeneratorWrapperCalculator");
  *wrapper_node.mutable_input_side_packet() = node.input_side_packet();
  *wrapper_node.mutable_output_side_packet() = node.output_side_packet();

  auto* wrapper_options = wrapper_node.mutable_options()->MutableExtension(
      mediapipe::PacketGeneratorWrapperCalculatorOptions::ext);
  wrapper_options->set_packet_generator(node.packet_generator());
  wrapper_options->set_package(package);
  if (node.has_options()) {
    *wrapper_options->mutable_options() = node.options();
  }
  return wrapper_node;
}

}  // anonymous namespace

absl::Status CalculatorContract::Initialize(
    const CalculatorGraphConfig::Node& node) {
  std::vector<absl::Status> statuses;

  auto input_stream_statusor = tool::TagMap::Create(node.input_stream());
  if (!input_stream_statusor.ok()) {
    statuses.push_back(std::move(input_stream_statusor).status());
  }
  auto output_stream_statusor = tool::TagMap::Create(node.output_stream());
  if (!output_stream_statusor.ok()) {
    statuses.push_back(std::move(output_stream_statusor).status());
  }
  auto input_side_packet_statusor =
      tool::TagMap::Create(node.input_side_packet());
  if (!input_side_packet_statusor.ok()) {
    statuses.push_back(std::move(input_side_packet_statusor).status());
  }
  auto output_side_packet_statusor =
      tool::TagMap::Create(node.output_side_packet());
  if (!output_side_packet_statusor.ok()) {
    statuses.push_back(std::move(output_side_packet_statusor).status());
  }

  if (!statuses.empty()) {
    auto builder = mediapipe::UnknownErrorBuilder(MEDIAPIPE_LOC)
                   << "Unable to initialize TagMaps for node.";
    for (const auto& status : statuses) {
      builder << "\n" << status.message();
    }
#if !(defined(MEDIAPIPE_LITE) || defined(MEDIAPIPE_MOBILE))
    builder << "\nFor calculator:\n";
    builder << node.DebugString();
#endif  // !(MEDIAPIPE_LITE || MEDIAPIPE_MOBILE)
    return std::move(builder);
  }

  node_config_ = &node;
  options_.Initialize(*node_config_);
  // Create the PacketTypeSets.
  inputs_ = absl::make_unique<PacketTypeSet>(
      std::move(input_stream_statusor).value());
  outputs_ = absl::make_unique<PacketTypeSet>(
      std::move(output_stream_statusor).value());
  input_side_packets_ = absl::make_unique<PacketTypeSet>(
      std::move(input_side_packet_statusor).value());
  output_side_packets_ = absl::make_unique<PacketTypeSet>(
      std::move(output_side_packet_statusor).value());
  return absl::OkStatus();
}

absl::Status CalculatorContract::Initialize(const PacketGeneratorConfig& node,
                                            const std::string& package) {
  std::vector<absl::Status> statuses;

  auto input_side_packet_statusor =
      tool::TagMap::Create(node.input_side_packet());
  if (!input_side_packet_statusor.ok()) {
    statuses.push_back(std::move(input_side_packet_statusor).status());
  }
  auto output_side_packet_statusor =
      tool::TagMap::Create(node.output_side_packet());
  if (!output_side_packet_statusor.ok()) {
    statuses.push_back(std::move(output_side_packet_statusor).status());
  }

  if (!statuses.empty()) {
    auto builder = UnknownErrorBuilder(MEDIAPIPE_LOC)
                   << "NodeTypeInfo Initialization failed.";
    for (const auto& status : statuses) {
      builder << "\n" << status.message();
    }
#if !(defined(MEDIAPIPE_LITE) || defined(MEDIAPIPE_MOBILE))
    builder << "\nFor packet_generator:\n";
    builder << node.DebugString();
#endif  // !(MEDIAPIPE_LITE || MEDIAPIPE_MOBILE)
    return std::move(builder);
  }

  wrapper_config_ = std::make_unique<CalculatorGraphConfig::Node>(
      MakePacketGeneratorWrapperConfig(node, package));
  options_.Initialize(*wrapper_config_);
  inputs_ = absl::make_unique<PacketTypeSet>(0);
  outputs_ = absl::make_unique<PacketTypeSet>(0);
  input_side_packets_ = absl::make_unique<PacketTypeSet>(
      std::move(input_side_packet_statusor).value());
  output_side_packets_ = absl::make_unique<PacketTypeSet>(
      std::move(output_side_packet_statusor).value());
  return absl::OkStatus();
}

absl::Status CalculatorContract::Initialize(const StatusHandlerConfig& node) {
  std::vector<absl::Status> statuses;

  auto input_side_packet_statusor =
      tool::TagMap::Create(node.input_side_packet());
  if (!input_side_packet_statusor.ok()) {
    statuses.push_back(std::move(input_side_packet_statusor).status());
  }

  if (!statuses.empty()) {
    auto builder = mediapipe::UnknownErrorBuilder(MEDIAPIPE_LOC)
                   << "NodeTypeInfo Initialization failed.";
    for (const auto& status : statuses) {
      builder << "\n" << status.message();
    }
#if !(defined(MEDIAPIPE_LITE) || defined(MEDIAPIPE_MOBILE))
    builder << "\nFor status_handler:\n";
    builder << node.DebugString();
#endif  // !(MEDIAPIPE_LITE || MEDIAPIPE_MOBILE)
    return std::move(builder);
  }

  input_side_packets_ = absl::make_unique<PacketTypeSet>(
      std::move(input_side_packet_statusor).value());
  return absl::OkStatus();
}

}  // namespace mediapipe
