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

#include "mediapipe/framework/legacy_calculator_support.h"

namespace mediapipe {

template <>
thread_local CalculatorContext*
    LegacyCalculatorSupport::Scoped<CalculatorContext>::current_ = nullptr;
template <>
thread_local CalculatorContract*
    LegacyCalculatorSupport::Scoped<CalculatorContract>::current_ = nullptr;

}  // namespace mediapipe
