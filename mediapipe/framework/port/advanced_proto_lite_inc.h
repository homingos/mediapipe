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

// advanced_proto_inc.h includes all the proto header files used by mediapipe
// framework code, and it should only be used by a limited number of files that
// do advanced proto template parsing and I/O.
#ifndef MEDIAPIPE_PORT_ADVANCED_PROTO_LITE_INC_H_
#define MEDIAPIPE_PORT_ADVANCED_PROTO_LITE_INC_H_

#include "google/protobuf/io/zero_copy_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/wire_format_lite.h"
#include "mediapipe/framework/port.h"
#include "mediapipe/framework/port/core_proto_inc.h"
#include "mediapipe/framework/port/proto_ns.h"

namespace mediapipe {
using proto_int64 = google::protobuf::int64;
using proto_uint64 = google::protobuf::uint64;
}  // namespace mediapipe

#endif  // MEDIAPIPE_PORT_ADVANCED_PROTO_LITE_INC_H_
