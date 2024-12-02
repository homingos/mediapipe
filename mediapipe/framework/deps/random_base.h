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

#ifndef MEDIAPIPE_DEPS_RANDOM_BASE_H_
#define MEDIAPIPE_DEPS_RANDOM_BASE_H_

#include <cstdint>

class RandomBase {
 public:
  // constructors.  Don't do too much.
  RandomBase() {}
  virtual ~RandomBase();

  virtual float RandFloat() { return 0; }
  virtual int UnbiasedUniform(int n) { return 0; }
  virtual uint64_t UnbiasedUniform64(uint64_t n) { return 0; }
};

#endif  // MEDIAPIPE_DEPS_RANDOM_BASE_H_
