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

#ifndef MEDIAPIPE_FRAMEWORK_TOOL_VALIDATE_H_
#define MEDIAPIPE_FRAMEWORK_TOOL_VALIDATE_H_

#include <string>

#include "mediapipe/framework/calculator.pb.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {

namespace tool {

// Returns absl::OkStatus() if the InputCollection is valid.  An input
// collection is invalid if it does not have the proper fields set
// depending on what its input_type field is.  Furthermore, if it uses
// INLINE, then the number of value fields in each inputs must match
// the number of input_side_packet_name fields.
absl::Status ValidateInput(const InputCollection& input);

}  // namespace tool
}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_TOOL_VALIDATE_H_
