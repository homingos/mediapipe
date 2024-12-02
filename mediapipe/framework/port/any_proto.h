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

#ifndef MEDIAPIPE_PORT_ANY_PROTO_H_
#define MEDIAPIPE_PORT_ANY_PROTO_H_

#include "mediapipe/framework/port/core_proto_inc.h"

namespace mediapipe {
namespace protobuf {

#if !defined(MEDIAPIPE_PROTO_LITE) || !defined(MEDIAPIPE_PROTO_THIRD_PARTY)
// The full definition of protobuf::Any for most platforms.
using Any = google::protobuf::Any;
#else
// A dummy definition of protobuf::Any for third_party/protobuf:protobuf-lite.
class Any {
 public:
  bool UnpackTo(proto_ns::Message* message) const { return false; }
  template <typename T>
  bool Is() const {
    return false;
  }
  absl::string_view type_url() const { return ""; }
  static const Any& default_instance() {
    static Any _Any_default_instance_;
    return _Any_default_instance_;
  }
};
#endif

}  // namespace protobuf
}  // namespace mediapipe

#endif  // MEDIAPIPE_PORT_ANY_PROTO_H_
