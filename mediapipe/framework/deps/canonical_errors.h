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

#ifndef MEDIAPIPE_DEPS_CANONICAL_ERRORS_H_
#define MEDIAPIPE_DEPS_CANONICAL_ERRORS_H_

#include "mediapipe/framework/deps/status.h"

namespace mediapipe {

// Each of the functions below creates a canonical error with the given
// message. The error code of the returned status object matches the name of
// the function.
inline absl::Status AlreadyExistsError(absl::string_view message) {
  return absl::Status(absl::StatusCode::kAlreadyExists, message);
}

inline absl::Status CancelledError() {
  return absl::Status(absl::StatusCode::kCancelled, "");
}

inline absl::Status CancelledError(absl::string_view message) {
  return absl::Status(absl::StatusCode::kCancelled, message);
}

inline absl::Status InternalError(absl::string_view message) {
  return absl::Status(absl::StatusCode::kInternal, message);
}

inline absl::Status InvalidArgumentError(absl::string_view message) {
  return absl::Status(absl::StatusCode::kInvalidArgument, message);
}

inline absl::Status FailedPreconditionError(absl::string_view message) {
  return absl::Status(absl::StatusCode::kFailedPrecondition, message);
}

inline absl::Status NotFoundError(absl::string_view message) {
  return absl::Status(absl::StatusCode::kNotFound, message);
}

inline absl::Status OutOfRangeError(absl::string_view message) {
  return absl::Status(absl::StatusCode::kOutOfRange, message);
}

inline absl::Status PermissionDeniedError(absl::string_view message) {
  return absl::Status(absl::StatusCode::kPermissionDenied, message);
}

inline absl::Status UnimplementedError(absl::string_view message) {
  return absl::Status(absl::StatusCode::kUnimplemented, message);
}

inline absl::Status UnknownError(absl::string_view message) {
  return absl::Status(absl::StatusCode::kUnknown, message);
}

inline absl::Status UnavailableError(absl::string_view message) {
  return absl::Status(absl::StatusCode::kUnavailable, message);
}

inline bool IsCancelled(const absl::Status& status) {
  return status.code() == absl::StatusCode::kCancelled;
}

inline bool IsNotFound(const absl::Status& status) {
  return status.code() == absl::StatusCode::kNotFound;
}

}  // namespace mediapipe

#endif  // MEDIAPIPE_DEPS_CANONICAL_ERRORS_H_
