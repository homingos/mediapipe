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

#include "mediapipe/framework/tool/validate.h"

#include <string>

#include "mediapipe/framework/calculator.pb.h"
#include "mediapipe/framework/port/canonical_errors.h"
#include "mediapipe/framework/port/core_proto_inc.h"
#include "mediapipe/framework/port/status_macros.h"
#include "mediapipe/framework/tool/validate_name.h"

namespace mediapipe {

namespace tool {

absl::Status ValidateInput(const InputCollection& input_collection) {
  if (!input_collection.name().empty()) {
    MP_RETURN_IF_ERROR(tool::ValidateName(input_collection.name())).SetPrepend()
        << "InputCollection " << input_collection.name()
        << " has improperly specified name: ";
  }
  if (input_collection.input_type() <= InputCollection::UNKNOWN ||
      input_collection.input_type() >= InputCollection::INVALID_UPPER_BOUND) {
    return absl::InvalidArgumentError(
        "InputCollection must specify a valid input_type.");
  }
  if (input_collection.file_name().empty()) {
    return absl::InvalidArgumentError(
        "InputCollection must specify a file_name.");
  }
  return absl::OkStatus();
}

}  // namespace tool
}  // namespace mediapipe
