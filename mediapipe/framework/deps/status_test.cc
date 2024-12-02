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

#include "mediapipe/framework/deps/status.h"

#include "mediapipe/framework/deps/status_matchers.h"
#include "mediapipe/framework/port/gtest.h"

namespace mediapipe {

TEST(Status, OK) {
  EXPECT_EQ(OkStatus().code(), absl::StatusCode::kOk);
  EXPECT_EQ(OkStatus().message(), "");
  MP_EXPECT_OK(OkStatus());
  MP_ASSERT_OK(OkStatus());
  EXPECT_EQ(OkStatus(), Status());
  Status s;
  EXPECT_TRUE(s.ok());
}

TEST(DeathStatus, CheckOK) {
  Status status(absl::StatusCode::kInvalidArgument, "Invalid");
  ASSERT_DEATH(MEDIAPIPE_CHECK_OK(status), "Invalid");
}

TEST(Status, Set) {
  Status status;
  status = Status(absl::StatusCode::kCancelled, "Error message");
  EXPECT_EQ(status.code(), absl::StatusCode::kCancelled);
  EXPECT_EQ(status.message(), "Error message");
}

TEST(Status, Copy) {
  Status a(absl::StatusCode::kInvalidArgument, "Invalid");
  Status b(a);
  ASSERT_EQ(a.ToString(), b.ToString());
}

TEST(Status, Assign) {
  Status a(absl::StatusCode::kInvalidArgument, "Invalid");
  Status b;
  b = a;
  ASSERT_EQ(a.ToString(), b.ToString());
}

TEST(Status, Update) {
  Status s;
  s.Update(OkStatus());
  ASSERT_TRUE(s.ok());
  Status a(absl::StatusCode::kInvalidArgument, "Invalid");
  s.Update(a);
  ASSERT_EQ(s.ToString(), a.ToString());
  Status b(absl::StatusCode::kInternal, "Invalid");
  s.Update(b);
  ASSERT_EQ(s.ToString(), a.ToString());
  s.Update(OkStatus());
  ASSERT_EQ(s.ToString(), a.ToString());
  ASSERT_FALSE(s.ok());
}

TEST(Status, EqualsOK) { ASSERT_EQ(OkStatus(), Status()); }

TEST(Status, EqualsSame) {
  Status a(absl::StatusCode::kInvalidArgument, "Invalid");
  Status b(absl::StatusCode::kInvalidArgument, "Invalid");
  ASSERT_EQ(a, b);
}

TEST(Status, EqualsCopy) {
  const Status a(absl::StatusCode::kInvalidArgument, "Invalid");
  const Status b = a;
  ASSERT_EQ(a, b);
}

TEST(Status, EqualsDifferentCode) {
  const Status a(absl::StatusCode::kInvalidArgument, "Invalid");
  const Status b(absl::StatusCode::kInternal, "Internal");
  ASSERT_NE(a, b);
}

TEST(Status, EqualsDifferentMessage) {
  const Status a(absl::StatusCode::kInvalidArgument, "message");
  const Status b(absl::StatusCode::kInvalidArgument, "another");
  ASSERT_NE(a, b);
}

}  // namespace mediapipe
