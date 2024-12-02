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
//
// The abstract class of counter.

#ifndef MEDIAPIPE_FRAMEWORK_COUNTER_H_
#define MEDIAPIPE_FRAMEWORK_COUNTER_H_

#include <cstdint>

namespace mediapipe {

class Counter {
 public:
  Counter() {}
  virtual ~Counter() {}

  virtual void Increment() = 0;
  virtual void IncrementBy(int amount) = 0;
  virtual int64_t Get() = 0;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_COUNTER_H_
