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

#ifndef MEDIAPIPE_DEPS_RET_CHECK_H_
#define MEDIAPIPE_DEPS_RET_CHECK_H_

#include "absl/base/optimization.h"
#include "mediapipe/framework/deps/status_builder.h"
#include "mediapipe/framework/deps/status_macros.h"

namespace mediapipe {
// Returns a StatusBuilder that corresponds to a `RET_CHECK` failure.
mediapipe::StatusBuilder RetCheckFailSlowPath(
    mediapipe::source_location location);

// Returns a StatusBuilder that corresponds to a `RET_CHECK` failure.
mediapipe::StatusBuilder RetCheckFailSlowPath(
    mediapipe::source_location location, const char* condition);

// Returns a StatusBuilder that corresponds to a `RET_CHECK` failure.
mediapipe::StatusBuilder RetCheckFailSlowPath(
    mediapipe::source_location location, const char* condition,
    const absl::Status& status);

inline StatusBuilder RetCheckImpl(const absl::Status& status,
                                  const char* condition,
                                  mediapipe::source_location location) {
  if (ABSL_PREDICT_TRUE(status.ok()))
    return mediapipe::StatusBuilder(absl::OkStatus(), location);
  return RetCheckFailSlowPath(location, condition, status);
}

}  // namespace mediapipe

#define RET_CHECK(cond)               \
  while (ABSL_PREDICT_FALSE(!(cond))) \
  return mediapipe::RetCheckFailSlowPath(MEDIAPIPE_LOC, #cond)

#define RET_CHECK_OK(status) \
  MP_RETURN_IF_ERROR(mediapipe::RetCheckImpl((status), #status, MEDIAPIPE_LOC))

#define RET_CHECK_FAIL() return mediapipe::RetCheckFailSlowPath(MEDIAPIPE_LOC)

#define MEDIAPIPE_INTERNAL_RET_CHECK_OP(name, op, lhs, rhs) \
  RET_CHECK((lhs)op(rhs))

#define RET_CHECK_EQ(lhs, rhs) MEDIAPIPE_INTERNAL_RET_CHECK_OP(EQ, ==, lhs, rhs)
#define RET_CHECK_NE(lhs, rhs) MEDIAPIPE_INTERNAL_RET_CHECK_OP(NE, !=, lhs, rhs)
#define RET_CHECK_LE(lhs, rhs) MEDIAPIPE_INTERNAL_RET_CHECK_OP(LE, <=, lhs, rhs)
#define RET_CHECK_LT(lhs, rhs) MEDIAPIPE_INTERNAL_RET_CHECK_OP(LT, <, lhs, rhs)
#define RET_CHECK_GE(lhs, rhs) MEDIAPIPE_INTERNAL_RET_CHECK_OP(GE, >=, lhs, rhs)
#define RET_CHECK_GT(lhs, rhs) MEDIAPIPE_INTERNAL_RET_CHECK_OP(GT, >, lhs, rhs)

#endif  // MEDIAPIPE_DEPS_RET_CHECK_H_
