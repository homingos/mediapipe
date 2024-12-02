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

#include "mediapipe/framework/test_service.h"

namespace mediapipe {

absl::Status TestServiceCalculator::GetContract(CalculatorContract* cc) {
  cc->Inputs().Index(0).Set<int>();
  cc->Outputs().Index(0).SetSameAs(&cc->Inputs().Index(0));
  // This service will be required. The graph won't start without it.
  cc->UseService(kTestService);
  // This service is optional for this calculator.
  cc->UseService(kAnotherService).Optional();
  return absl::OkStatus();
}

absl::Status TestServiceCalculator::Open(CalculatorContext* cc) {
  cc->SetOffset(TimestampDiff(0));
  // For an optional service, check whether it's available.
  if (cc->Service(kAnotherService).IsAvailable()) {
    optional_bias_ = cc->Service(kAnotherService).GetObject();
  }
  return absl::OkStatus();
}

absl::Status TestServiceCalculator::Process(CalculatorContext* cc) {
  int value = cc->Inputs().Index(0).Value().Get<int>();
  // A required service is sure to be available, so we can just GetObject.
  TestServiceObject& service_object = cc->Service(kTestService).GetObject();
  int delta = service_object["delta"];
  service_object["count"] += 1;
  int x = value + delta + optional_bias_;
  cc->Outputs().Index(0).Add(new int(x), cc->InputTimestamp());
  return absl::OkStatus();
}

REGISTER_CALCULATOR(TestServiceCalculator);

}  // namespace mediapipe
