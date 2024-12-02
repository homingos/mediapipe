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

// Definitions for CalculatorBase.

#include "mediapipe/framework/calculator_base.h"

#include <algorithm>

namespace mediapipe {

CalculatorBase::CalculatorBase() {}

CalculatorBase::~CalculatorBase() {}

Timestamp CalculatorBase::SourceProcessOrder(
    const CalculatorContext* cc) const {
  Timestamp result = Timestamp::Max();
  for (const OutputStreamShard& output : cc->Outputs()) {
    result = std::min(result, output.NextTimestampBound());
  }
  return result;
}

}  // namespace mediapipe
