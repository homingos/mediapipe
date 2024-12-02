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

#include "mediapipe/framework/packet_generator.h"

#include "absl/strings/str_cat.h"
#include "mediapipe/framework/packet_generator.pb.h"
#include "mediapipe/framework/packet_type.h"
#include "mediapipe/framework/port/gmock.h"
#include "mediapipe/framework/port/gtest.h"
#include "mediapipe/framework/port/status_matchers.h"
#include "mediapipe/framework/tool/validate_type.h"

namespace mediapipe {

namespace {
class DoNothingGenerator : public PacketGenerator {
 public:
  static absl::Status FillExpectations(
      const PacketGeneratorOptions& extendable_options,
      PacketTypeSet* input_side_packets, PacketTypeSet* output_side_packets) {
    for (CollectionItemId id = input_side_packets->BeginId();
         id < input_side_packets->EndId(); ++id) {
      input_side_packets->Get(id).SetAny();
    }
    for (CollectionItemId id = output_side_packets->BeginId();
         id < output_side_packets->EndId(); ++id) {
      output_side_packets->Get(id).Set<bool>();
    }
    return absl::OkStatus();
  }

  static absl::Status Generate(const PacketGeneratorOptions& extendable_options,
                               const PacketSet& input_side_packets,
                               PacketSet* output_side_packets) {
    for (CollectionItemId id = output_side_packets->BeginId();
         id < output_side_packets->EndId(); ++id) {
      output_side_packets->Get(id) = MakePacket<bool>(true);
    }
    return absl::OkStatus();
  }
};

REGISTER_PACKET_GENERATOR(DoNothingGenerator);

TEST(PacketGeneratorTest, FillExpectationsOnConfig) {
  PacketGeneratorConfig config;
  config.set_packet_generator("DoNothingGenerator");
  config.add_input_side_packet("any");
  config.add_input_side_packet("number");
  config.add_input_side_packet("of_inputs");
  config.add_output_side_packet("any_number_of");
  config.add_output_side_packet("output_side_packets");
  MP_EXPECT_OK(tool::RunGeneratorFillExpectations(config));
}

}  // namespace
}  // namespace mediapipe
