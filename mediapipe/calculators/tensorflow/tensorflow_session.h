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

#ifndef MEDIAPIPE_TENSORFLOW_CALCULATORS_TENSORFLOW_SESSION_H_
#define MEDIAPIPE_TENSORFLOW_CALCULATORS_TENSORFLOW_SESSION_H_

#include <memory>

#include "tensorflow/core/public/session.h"

namespace mediapipe {
struct TensorFlowSession {
  // TensorFlow session wrapper to get around the RTTI issue.
  std::unique_ptr<tensorflow::Session> session;

  // Store an optional mapping to the between MediaPipe tags and TensorFlow
  // tensor names. Creating this mapping when the session is loaded allows more
  // flexible definition of mapping tags to tensors across platforms.
  std::map<std::string, std::string> tag_to_tensor_map;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_TENSORFLOW_CALCULATORS_TENSORFLOW_SESSION_H_
