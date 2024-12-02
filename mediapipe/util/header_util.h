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

#ifndef MEDIAPIPE_UTIL_HEADER_UTIL_H_
#define MEDIAPIPE_UTIL_HEADER_UTIL_H_

#include "mediapipe/framework/packet_set.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {

// Copies headers from |inputs| into |outputs| respectively. The size of
// |inputs| and |outputs| must be equal.
absl::Status CopyInputHeadersToOutputs(const InputStreamSet& inputs,
                                       const OutputStreamSet& outputs);

absl::Status CopyInputHeadersToOutputs(const InputStreamShardSet& inputs,
                                       OutputStreamShardSet* outputs);

}  // namespace mediapipe

#endif  // MEDIAPIPE_UTIL_HEADER_UTIL_H_
