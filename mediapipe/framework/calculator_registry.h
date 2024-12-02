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
// Calculator registration.

#ifndef MEDIAPIPE_FRAMEWORK_CALCULATOR_REGISTRY_H_
#define MEDIAPIPE_FRAMEWORK_CALCULATOR_REGISTRY_H_

#include "mediapipe/framework/calculator_base.h"

// Macro for registering calculators.
#define REGISTER_CALCULATOR(name)                                       \
  REGISTER_FACTORY_FUNCTION_QUALIFIED(                                  \
      mediapipe::CalculatorBaseRegistry, calculator_registration, name, \
      absl::make_unique<mediapipe::internal::CalculatorBaseFactoryFor<name>>)

#endif  // MEDIAPIPE_FRAMEWORK_CALCULATOR_REGISTRY_H_
