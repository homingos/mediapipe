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

#include "mediapipe/framework/port/advanced_proto_inc.h"
#include "mediapipe/framework/tool/options_registry.h"
#include "{{MESSAGE_NAME_HEADER}}"
#include "{{MESSAGE_PROTO_HEADER}}"

namespace {
constexpr char kDescriptorContents[] =
#include "{{DESCRIPTOR_INC_FILE_PATH}}"
    ;  // NOLINT(whitespace/semicolon)

mediapipe::FieldData ReadFileDescriptorSet(const std::string& pb) {
  mediapipe::FieldData result;
  *result.mutable_message_value()->mutable_type_url() =
      "google::protobuf.FileDescriptorSet";
  *result.mutable_message_value()->mutable_value() = pb;

  // Force linking of the generated options protobuf.
  mediapipe::proto_ns::LinkMessageReflection<
      MP_OPTION_TYPE_NS::MP_OPTION_TYPE_NAME>();
  return result;
}

}  // namespace

namespace mediapipe {
// The protobuf descriptor for an options message type.
template <>
const RegistrationToken tool::OptionsRegistry::registration_token<
    MP_OPTION_TYPE_NS::MP_OPTION_TYPE_NAME> =
    tool::OptionsRegistry::Register(ReadFileDescriptorSet(
        std::string(kDescriptorContents, sizeof(kDescriptorContents) - 1)));
}  // namespace mediapipe
