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

#ifndef MEDIAPIPE_GPU_GPU_TEST_BASE_H_
#define MEDIAPIPE_GPU_GPU_TEST_BASE_H_

#include <functional>
#include <memory>

#include "mediapipe/framework/port/gmock.h"
#include "mediapipe/framework/port/gtest.h"
#include "mediapipe/gpu/gl_calculator_helper.h"
#include "mediapipe/gpu/gpu_shared_data_internal.h"

namespace mediapipe {

class GpuTestEnvironment {
 protected:
  GpuTestEnvironment() { helper_.InitializeForTest(gpu_resources_.get()); }

  void RunInGlContext(std::function<void(void)> gl_func) {
    helper_.RunInGlContext(std::move(gl_func));
  }

  GpuSharedData gpu_shared_;
  std::shared_ptr<GpuResources> gpu_resources_ = gpu_shared_.gpu_resources;
  GlCalculatorHelper helper_;
};

class GpuTestBase : public testing::Test, public GpuTestEnvironment {};

template <typename T>
class GpuTestWithParamBase : public testing::TestWithParam<T>,
                             public GpuTestEnvironment {};

}  // namespace mediapipe

#endif  // MEDIAPIPE_GPU_GPU_TEST_BASE_H_
