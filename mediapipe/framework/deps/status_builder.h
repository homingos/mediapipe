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

#ifndef MEDIAPIPE_DEPS_STATUS_BUILDER_H_
#define MEDIAPIPE_DEPS_STATUS_BUILDER_H_

#include <memory>
#include <sstream>
#include <utility>

#include "absl/base/attributes.h"
#include "absl/memory/memory.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "mediapipe/framework/deps/source_location.h"
#include "mediapipe/framework/deps/status.h"

namespace mediapipe {

class ABSL_MUST_USE_RESULT StatusBuilder {
 public:
  StatusBuilder(const StatusBuilder& sb);
  StatusBuilder& operator=(const StatusBuilder& sb);

  StatusBuilder(StatusBuilder&&) = default;
  StatusBuilder& operator=(StatusBuilder&&) = default;

  // Creates a `StatusBuilder` based on an original status.  If logging is
  // enabled, it will use `location` as the location from which the log message
  // occurs.  A typical user will call this with `MEDIAPIPE_LOC`.
  StatusBuilder(const absl::Status& original_status,
                mediapipe::source_location location)
      : impl_(original_status.ok()
                  ? nullptr
                  : std::make_unique<Impl>(original_status, location)) {}

  StatusBuilder(absl::Status&& original_status,
                mediapipe::source_location location)
      : impl_(original_status.ok()
                  ? nullptr
                  : std::make_unique<Impl>(std::move(original_status),
                                           location)) {}

  // Creates a `StatusBuilder` from a mediapipe status code.  If logging is
  // enabled, it will use `location` as the location from which the log message
  // occurs.  A typical user will call this with `MEDIAPIPE_LOC`.
  StatusBuilder(absl::StatusCode code, mediapipe::source_location location)
      : impl_(code == absl::StatusCode::kOk
                  ? nullptr
                  : std::make_unique<Impl>(absl::Status(code, ""), location)) {}

  bool ok() const { return !impl_; }

  StatusBuilder& SetAppend() &;
  StatusBuilder&& SetAppend() &&;

  StatusBuilder& SetPrepend() &;
  StatusBuilder&& SetPrepend() &&;

  StatusBuilder& SetNoLogging() &;
  StatusBuilder&& SetNoLogging() &&;

  StatusBuilder& SetCode(absl::StatusCode code) &;
  StatusBuilder&& SetCode(absl::StatusCode code) &&;

  template <typename T>
  StatusBuilder& operator<<(const T& msg) & {
    if (!impl_) return *this;
    impl_->stream << msg;
    return *this;
  }

  template <typename T>
  StatusBuilder&& operator<<(const T& msg) && {
    return std::move(*this << msg);
  }

  operator absl::Status() const&;
  operator absl::Status() &&;

  absl::Status JoinMessageToStatus();

 private:
  struct Impl {
    // Specifies how to join the error message in the original status and any
    // additional message that has been streamed into the builder.
    enum class MessageJoinStyle {
      kAnnotate,
      kAppend,
      kPrepend,
    };

    Impl(const absl::Status& status, mediapipe::source_location location);
    Impl(absl::Status&& status, mediapipe::source_location location);
    Impl(const Impl&);
    Impl& operator=(const Impl&);

    absl::Status JoinMessageToStatus();

    // The status that the result will be based on.
    absl::Status status;
    // The source location to record if this file is logged.
    mediapipe::source_location location;
    // Logging disabled if true.
    bool no_logging = false;
    // The additional messages added with `<<`.  This is nullptr when status_ is
    // ok.
    std::ostringstream stream;
    // Specifies how to join the message in `status_` and `stream_`.
    MessageJoinStyle join_style = MessageJoinStyle::kAnnotate;
  };

  // Internal store of data for the class.  An invariant of the class is that
  // this is null when the original status is okay, and not-null otherwise.
  std::unique_ptr<Impl> impl_;
};

inline StatusBuilder AlreadyExistsErrorBuilder(
    mediapipe::source_location location) {
  return StatusBuilder(absl::StatusCode::kAlreadyExists, location);
}

inline StatusBuilder FailedPreconditionErrorBuilder(
    mediapipe::source_location location) {
  return StatusBuilder(absl::StatusCode::kFailedPrecondition, location);
}

inline StatusBuilder InternalErrorBuilder(mediapipe::source_location location) {
  return StatusBuilder(absl::StatusCode::kInternal, location);
}

inline StatusBuilder InvalidArgumentErrorBuilder(
    mediapipe::source_location location) {
  return StatusBuilder(absl::StatusCode::kInvalidArgument, location);
}

inline StatusBuilder NotFoundErrorBuilder(mediapipe::source_location location) {
  return StatusBuilder(absl::StatusCode::kNotFound, location);
}

inline StatusBuilder UnavailableErrorBuilder(
    mediapipe::source_location location) {
  return StatusBuilder(absl::StatusCode::kUnavailable, location);
}

inline StatusBuilder UnimplementedErrorBuilder(
    mediapipe::source_location location) {
  return StatusBuilder(absl::StatusCode::kUnimplemented, location);
}

inline StatusBuilder UnknownErrorBuilder(mediapipe::source_location location) {
  return StatusBuilder(absl::StatusCode::kUnknown, location);
}

}  // namespace mediapipe

#endif  // MEDIAPIPE_DEPS_STATUS_BUILDER_H_
