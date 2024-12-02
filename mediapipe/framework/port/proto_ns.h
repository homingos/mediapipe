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

#ifndef MEDIAPIPE_PORT_PROTO_NS_H_
#define MEDIAPIPE_PORT_PROTO_NS_H_

#include <string>

// Temporary forward declarations for proto2 support on portable targets.
// Use proto_ns inside namespace mediapipe instead of proto2 namespace.
#include "google/protobuf/message.h"
#include "google/protobuf/message_lite.h"
#include "google/protobuf/repeated_field.h"
#include "mediapipe/framework/port.h"

namespace mediapipe {
namespace proto_ns = ::google::protobuf;
typedef ::std::string ProtoString;
}  // namespace mediapipe.

// Legacy namespace support.
namespace mediapipe {
namespace proto_ns = mediapipe::proto_ns;
typedef ::std::string ProtoString;
}  // namespace mediapipe

#endif  // MEDIAPIPE_PORT_PROTO_NS_H_
