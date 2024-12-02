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

#include "mediapipe/framework/tool/status_util.h"

#include <vector>

#include "absl/log/absl_check.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_join.h"
#include "absl/strings/string_view.h"

namespace mediapipe {
namespace tool {

absl::Status StatusInvalid(absl::string_view message) {
  return absl::Status(absl::StatusCode::kInvalidArgument, message);
}

absl::Status StatusFail(absl::string_view message) {
  return absl::Status(absl::StatusCode::kUnknown, message);
}

absl::Status StatusStop() {
  return absl::Status(absl::StatusCode::kOutOfRange,
                      "mediapipe::tool::StatusStop()");
}

absl::Status AddStatusPrefix(absl::string_view prefix,
                             const absl::Status& status) {
  return absl::Status(status.code(), absl::StrCat(prefix, status.message()));
}

absl::Status CombinedStatus(absl::string_view general_comment,
                            const std::vector<absl::Status>& statuses) {
  // The final error code is absl::StatusCode::kUnknown if not all
  // the error codes are the same.  Otherwise it is the same error code
  // as all of the (non-OK) statuses.  If statuses is empty or they are
  // all OK, then absl::OkStatus() is returned.
  absl::StatusCode error_code = absl::StatusCode::kOk;
  std::vector<std::string> errors;
  for (const absl::Status& status : statuses) {
    if (!status.ok()) {
      errors.emplace_back(status.message());
      if (error_code == absl::StatusCode::kOk) {
        error_code = status.code();
      } else if (error_code != status.code()) {
        error_code = absl::StatusCode::kUnknown;
      }
    }
  }
  if (error_code == absl::StatusCode::kOk) return absl::OkStatus();
  absl::Status combined;
  combined = absl::Status(
      error_code,
      absl::StrCat(general_comment, "\n", absl::StrJoin(errors, "\n")));
  return combined;
}

}  // namespace tool
}  // namespace mediapipe
