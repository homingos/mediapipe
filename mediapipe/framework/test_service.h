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

#ifndef MEDIAPIPE_FRAMEWORK_TEST_SERVICE_H_
#define MEDIAPIPE_FRAMEWORK_TEST_SERVICE_H_

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/graph_service.h"

namespace mediapipe {

using TestServiceObject = std::map<std::string, int>;

inline constexpr GraphService<TestServiceObject> kTestService(
    "test_service", GraphServiceBase::kDisallowDefaultInitialization);
inline constexpr GraphService<int> kAnotherService(
    "another_service", GraphServiceBase::kAllowDefaultInitialization);

class NoDefaultConstructor {
 public:
  NoDefaultConstructor() = delete;
};
inline constexpr GraphService<NoDefaultConstructor> kNoDefaultService(
    "no_default_service", GraphServiceBase::kAllowDefaultInitialization);

class NeedsCreateMethod {
 public:
  static absl::StatusOr<std::shared_ptr<NeedsCreateMethod>> Create() {
    return std::shared_ptr<NeedsCreateMethod>(new NeedsCreateMethod());
  }

 private:
  NeedsCreateMethod() = default;
};
inline constexpr GraphService<NeedsCreateMethod> kNeedsCreateService(
    "needs_create_service", GraphServiceBase::kAllowDefaultInitialization);

// Use a service.
class TestServiceCalculator : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc);
  absl::Status Open(CalculatorContext* cc) final;
  absl::Status Process(CalculatorContext* cc) final;

 private:
  int optional_bias_ = 0;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_TEST_SERVICE_H_
