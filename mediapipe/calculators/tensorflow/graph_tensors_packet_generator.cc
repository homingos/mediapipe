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
// Generates row tensors of prescribed length that are initialized to zeros.
// The tensors are placed in an ordered map, which maps the tensors to the
// tensor tags, and emitted as a packet. This generator has been developed
// primarily to generate initialization states for LSTMs.

#include <map>
#include <string>

#include "mediapipe/calculators/tensorflow/graph_tensors_packet_generator.pb.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/tool/status_util.h"
#include "tensorflow/core/framework/tensor.h"

namespace mediapipe {

namespace tf = ::tensorflow;

class GraphTensorsPacketGenerator : public PacketGenerator {
 public:
  static absl::Status FillExpectations(
      const PacketGeneratorOptions& extendable_options,
      PacketTypeSet* input_side_packets, PacketTypeSet* output_side_packets) {
    RET_CHECK(extendable_options.HasExtension(
        GraphTensorsPacketGeneratorOptions::ext));
    const auto& options = extendable_options.GetExtension(  // NOLINT
        GraphTensorsPacketGeneratorOptions::ext);
    output_side_packets->Index(0)
        .Set<std::unique_ptr<std::map<std::string, tf::Tensor>>>(
            /* "A map of tensor tags and tensors" */);
    RET_CHECK_EQ(options.tensor_tag_size(), options.tensor_num_nodes_size());
    RET_CHECK_GT(options.tensor_tag_size(), 0);
    return absl::OkStatus();
  }

  static absl::Status Generate(
      const PacketGeneratorOptions& packet_generator_options,
      const PacketSet& input_side_packets, PacketSet* output_side_packets) {
    const GraphTensorsPacketGeneratorOptions& options =
        packet_generator_options.GetExtension(
            GraphTensorsPacketGeneratorOptions::ext);
    // Output bundle packet.
    auto tensor_map = absl::make_unique<std::map<std::string, tf::Tensor>>();

    for (int i = 0; i < options.tensor_tag_size(); ++i) {
      const std::string& tensor_tag = options.tensor_tag(i);
      const int32_t tensor_num_nodes = options.tensor_num_nodes(i);
      (*tensor_map)[tensor_tag] =
          tf::Tensor(tf::DT_FLOAT, tf::TensorShape{1, tensor_num_nodes});
      (*tensor_map)[tensor_tag].flat<float>().setZero();
    }
    output_side_packets->Index(0) = AdoptAsUniquePtr(tensor_map.release());
    return absl::OkStatus();
  }
};
REGISTER_PACKET_GENERATOR(GraphTensorsPacketGenerator);

}  // namespace mediapipe
