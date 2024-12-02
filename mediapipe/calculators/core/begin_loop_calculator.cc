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

#include "mediapipe/calculators/core/begin_loop_calculator.h"

#include <cstdint>
#include <string>
#include <vector>

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/detection.pb.h"
#include "mediapipe/framework/formats/image.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/matrix.h"
#include "mediapipe/framework/formats/rect.pb.h"
#include "mediapipe/framework/formats/tensor.h"
#include "mediapipe/gpu/gpu_buffer.h"

namespace mediapipe {

// A calculator to process std::vector<NormalizedLandmarkList>.
typedef BeginLoopCalculator<std::vector<::mediapipe::NormalizedLandmarkList>>
    BeginLoopNormalizedLandmarkListVectorCalculator;
REGISTER_CALCULATOR(BeginLoopNormalizedLandmarkListVectorCalculator);

// A calculator to process std::vector<int>.
typedef BeginLoopCalculator<std::vector<int>> BeginLoopIntCalculator;
REGISTER_CALCULATOR(BeginLoopIntCalculator);

// A calculator to process std::vector<NormalizedRect>.
typedef BeginLoopCalculator<std::vector<::mediapipe::NormalizedRect>>
    BeginLoopNormalizedRectCalculator;
REGISTER_CALCULATOR(BeginLoopNormalizedRectCalculator);

// A calculator to process std::vector<Detection>.
typedef BeginLoopCalculator<std::vector<::mediapipe::Detection>>
    BeginLoopDetectionCalculator;
REGISTER_CALCULATOR(BeginLoopDetectionCalculator);

// A calculator to process std::vector<Matrix>.
typedef BeginLoopCalculator<std::vector<Matrix>> BeginLoopMatrixCalculator;
REGISTER_CALCULATOR(BeginLoopMatrixCalculator);

// A calculator to process std::vector<std::vector<Matrix>>.
typedef BeginLoopCalculator<std::vector<std::vector<Matrix>>>
    BeginLoopMatrixVectorCalculator;
REGISTER_CALCULATOR(BeginLoopMatrixVectorCalculator);

// A calculator to process std::vector<uint64_t>.
typedef BeginLoopCalculator<std::vector<uint64_t>> BeginLoopUint64tCalculator;
REGISTER_CALCULATOR(BeginLoopUint64tCalculator);

// A calculator to process std::vector<mediapipe::Tensor>.
typedef BeginLoopCalculator<std::vector<Tensor>> BeginLoopTensorCalculator;
REGISTER_CALCULATOR(BeginLoopTensorCalculator);

// A calculator to process std::vector<mediapipe::ImageFrame>.
typedef BeginLoopCalculator<std::vector<ImageFrame>>
    BeginLoopImageFrameCalculator;
REGISTER_CALCULATOR(BeginLoopImageFrameCalculator);

// A calculator to process std::vector<mediapipe::GpuBuffer>.
typedef BeginLoopCalculator<std::vector<GpuBuffer>>
    BeginLoopGpuBufferCalculator;
REGISTER_CALCULATOR(BeginLoopGpuBufferCalculator);

// A calculator to process std::vector<mediapipe::Image>.
typedef BeginLoopCalculator<std::vector<Image>> BeginLoopImageCalculator;
REGISTER_CALCULATOR(BeginLoopImageCalculator);

// A calculator to process std::vector<float>.
typedef BeginLoopCalculator<std::vector<float>> BeginLoopFloatCalculator;
REGISTER_CALCULATOR(BeginLoopFloatCalculator);

// A calculator to process std::string.
typedef BeginLoopCalculator<std::vector<std::string>> BeginLoopStringCalculator;
REGISTER_CALCULATOR(BeginLoopStringCalculator);

}  // namespace mediapipe
