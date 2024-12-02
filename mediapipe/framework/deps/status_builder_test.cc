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

#include "mediapipe/framework/deps/status_builder.h"

#include "mediapipe/framework/port/gtest.h"

namespace mediapipe {

TEST(StatusBuilder, OkStatusLvalue) {
  StatusBuilder builder(absl::OkStatus(), MEDIAPIPE_LOC);
  builder << "annotated message1 " << "annotated message2";
  absl::Status status = builder;
  ASSERT_EQ(status, absl::OkStatus());
}

TEST(StatusBuilder, OkStatusRvalue) {
  absl::Status status = StatusBuilder(absl::OkStatus(), MEDIAPIPE_LOC)
                        << "annotated message1 " << "annotated message2";
  ASSERT_EQ(status, absl::OkStatus());
}

TEST(StatusBuilder, AnnotateMode) {
  absl::Status status = StatusBuilder(absl::Status(absl::StatusCode::kNotFound,
                                                   "original message"),
                                      MEDIAPIPE_LOC)
                        << "annotated message1 " << "annotated message2";
  ASSERT_FALSE(status.ok());
  EXPECT_EQ(status.code(), absl::StatusCode::kNotFound);
  EXPECT_EQ(status.message(),
            "original message; annotated message1 annotated message2");
}

TEST(StatusBuilder, PrependModeLvalue) {
  StatusBuilder builder(
      absl::Status(absl::StatusCode::kInvalidArgument, "original message"),
      MEDIAPIPE_LOC);
  builder.SetPrepend() << "prepended message1 " << "prepended message2 ";
  absl::Status status =
      StatusBuilder(
          absl::Status(absl::StatusCode::kInvalidArgument, "original message"),
          MEDIAPIPE_LOC)
          .SetPrepend()
      << "prepended message1 " << "prepended message2 ";
  ASSERT_FALSE(status.ok());
  EXPECT_EQ(status.code(), absl::StatusCode::kInvalidArgument);
  EXPECT_EQ(status.message(),
            "prepended message1 prepended message2 original message");
}

TEST(StatusBuilder, PrependModeRvalue) {
  absl::Status status =
      StatusBuilder(
          absl::Status(absl::StatusCode::kInvalidArgument, "original message"),
          MEDIAPIPE_LOC)
          .SetPrepend()
      << "prepended message1 " << "prepended message2 ";
  ASSERT_FALSE(status.ok());
  EXPECT_EQ(status.code(), absl::StatusCode::kInvalidArgument);
  EXPECT_EQ(status.message(),
            "prepended message1 prepended message2 original message");
}

TEST(StatusBuilder, AppendModeLvalue) {
  StatusBuilder builder(
      absl::Status(absl::StatusCode::kInternal, "original message"),
      MEDIAPIPE_LOC);
  builder.SetAppend() << " extra message1" << " extra message2";
  absl::Status status = builder;
  ASSERT_FALSE(status.ok());
  EXPECT_EQ(status.code(), absl::StatusCode::kInternal);
  EXPECT_EQ(status.message(), "original message extra message1 extra message2");
}

TEST(StatusBuilder, AppendModeRvalue) {
  absl::Status status = StatusBuilder(absl::Status(absl::StatusCode::kInternal,
                                                   "original message"),
                                      MEDIAPIPE_LOC)
                            .SetAppend()
                        << " extra message1" << " extra message2";
  ASSERT_FALSE(status.ok());
  EXPECT_EQ(status.code(), absl::StatusCode::kInternal);
  EXPECT_EQ(status.message(), "original message extra message1 extra message2");
}

TEST(StatusBuilder, NoLoggingModeLvalue) {
  StatusBuilder builder(
      absl::Status(absl::StatusCode::kUnavailable, "original message"),
      MEDIAPIPE_LOC);
  builder.SetNoLogging() << " extra message";
  absl::Status status = builder;
  ASSERT_FALSE(status.ok());
  EXPECT_EQ(status.code(), absl::StatusCode::kUnavailable);
  EXPECT_EQ(status.message(), "original message");
}

TEST(StatusBuilder, NoLoggingModeRvalue) {
  absl::Status status =
      StatusBuilder(
          absl::Status(absl::StatusCode::kUnavailable, "original message"),
          MEDIAPIPE_LOC)
          .SetNoLogging()
      << " extra message";
  ASSERT_FALSE(status.ok());
  EXPECT_EQ(status.code(), absl::StatusCode::kUnavailable);
  EXPECT_EQ(status.message(), "original message");
}

TEST(StatusBuilder, SetCodeLvalue) {
  StatusBuilder builder(
      absl::Status(absl::StatusCode::kUnavailable, "original message"),
      MEDIAPIPE_LOC);
  builder.SetCode(absl::StatusCode::kInternal);
  absl::Status status = builder;
  ASSERT_FALSE(status.ok());
  EXPECT_EQ(status.code(), absl::StatusCode::kInternal);
  EXPECT_EQ(status.message(), "original message");
}

TEST(StatusBuilder, SetCodeRvalue) {
  absl::Status status =
      StatusBuilder(
          absl::Status(absl::StatusCode::kUnavailable, "original message"),
          MEDIAPIPE_LOC)
          .SetCode(absl::StatusCode::kInternal);
  ASSERT_FALSE(status.ok());
  EXPECT_EQ(status.code(), absl::StatusCode::kInternal);
  EXPECT_EQ(status.message(), "original message");
}

}  // namespace mediapipe
