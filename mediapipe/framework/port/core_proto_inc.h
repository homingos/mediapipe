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
// core_proto_inc.h includes the most commonly used proto header files, and
// it can satisfy the majority of the use cases.
#ifndef MEDIAPIPE_PORT_CORE_PROTO_INC_H_
#define MEDIAPIPE_PORT_CORE_PROTO_INC_H_

#include "google/protobuf/io/tokenizer.h"
#include "google/protobuf/message_lite.h"
#include "google/protobuf/repeated_field.h"
#include "mediapipe/framework/port.h"
#include "mediapipe/framework/port/proto_ns.h"

#if !defined(MEDIAPIPE_PROTO_LITE)
#include "google/protobuf/text_format.h"
#endif  // !defined(MEDIAPIPE_PROTO_LITE)

#endif  // MEDIAPIPE_PORT_CORE_PROTO_INC_H_
