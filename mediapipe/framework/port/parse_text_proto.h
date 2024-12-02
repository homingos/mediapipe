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

#ifndef MEDIAPIPE_PORT_PARSE_TEXT_PROTO_H_
#define MEDIAPIPE_PORT_PARSE_TEXT_PROTO_H_

#include "absl/log/absl_check.h"
#include "mediapipe/framework/port/core_proto_inc.h"
#include "mediapipe/framework/port/logging.h"
#include "mediapipe/framework/port/proto_ns.h"

namespace mediapipe {

template <typename T>
bool ParseTextProto(const std::string& input, T* proto) {
  return proto_ns::TextFormat::ParseFromString(input, proto);
}

template <typename T>
T ParseTextProtoOrDie(const std::string& input) {
  T result;
  ABSL_CHECK(ParseTextProto(input, &result));
  return result;
}

}  // namespace mediapipe

#endif  // MEDIAPIPE_PORT_PARSE_TEXT_PROTO_H_
