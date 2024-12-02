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

#include "mediapipe/calculators/core/end_loop_calculator.h"

#include <array>
#include <utility>
#include <vector>

#include "mediapipe/framework/formats/classification.pb.h"
#include "mediapipe/framework/formats/detection.pb.h"
#include "mediapipe/framework/formats/image.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/matrix.h"
#include "mediapipe/framework/formats/rect.pb.h"
#include "mediapipe/framework/formats/tensor.h"
#include "mediapipe/gpu/gpu_buffer.h"
#include "mediapipe/util/render_data.pb.h"
#include "tensorflow/lite/interpreter.h"

namespace mediapipe {

typedef EndLoopCalculator<std::vector<::mediapipe::NormalizedRect>>
    EndLoopNormalizedRectCalculator;
REGISTER_CALCULATOR(EndLoopNormalizedRectCalculator);

typedef EndLoopCalculator<std::vector<::mediapipe::LandmarkList>>
    EndLoopLandmarkListVectorCalculator;
REGISTER_CALCULATOR(EndLoopLandmarkListVectorCalculator);

typedef EndLoopCalculator<std::vector<::mediapipe::NormalizedLandmarkList>>
    EndLoopNormalizedLandmarkListVectorCalculator;
REGISTER_CALCULATOR(EndLoopNormalizedLandmarkListVectorCalculator);

typedef EndLoopCalculator<std::vector<bool>> EndLoopBooleanCalculator;
REGISTER_CALCULATOR(EndLoopBooleanCalculator);

typedef EndLoopCalculator<std::vector<float>> EndLoopFloatCalculator;
REGISTER_CALCULATOR(EndLoopFloatCalculator);

typedef EndLoopCalculator<std::vector<::mediapipe::RenderData>>
    EndLoopRenderDataCalculator;
REGISTER_CALCULATOR(EndLoopRenderDataCalculator);

typedef EndLoopCalculator<std::vector<::mediapipe::ClassificationList>>
    EndLoopClassificationListCalculator;
REGISTER_CALCULATOR(EndLoopClassificationListCalculator);

typedef EndLoopCalculator<std::vector<TfLiteTensor>>
    EndLoopTfLiteTensorCalculator;
REGISTER_CALCULATOR(EndLoopTfLiteTensorCalculator);

typedef EndLoopCalculator<std::vector<::mediapipe::Detection>>
    EndLoopDetectionCalculator;
REGISTER_CALCULATOR(EndLoopDetectionCalculator);

typedef EndLoopCalculator<std::vector<Matrix>> EndLoopMatrixCalculator;
REGISTER_CALCULATOR(EndLoopMatrixCalculator);

typedef EndLoopCalculator<std::vector<Tensor>> EndLoopTensorCalculator;
REGISTER_CALCULATOR(EndLoopTensorCalculator);

typedef EndLoopCalculator<std::vector<ImageFrame>> EndLoopImageFrameCalculator;
REGISTER_CALCULATOR(EndLoopImageFrameCalculator);

typedef EndLoopCalculator<std::vector<GpuBuffer>> EndLoopGpuBufferCalculator;
REGISTER_CALCULATOR(EndLoopGpuBufferCalculator);

typedef EndLoopCalculator<std::vector<::mediapipe::Image>>
    EndLoopImageCalculator;
REGISTER_CALCULATOR(EndLoopImageCalculator);

typedef EndLoopCalculator<std::vector<std::array<float, 16>>>
    EndLoopAffineMatrixCalculator;
REGISTER_CALCULATOR(EndLoopAffineMatrixCalculator);

typedef EndLoopCalculator<std::vector<std::pair<int, int>>>
    EndLoopImageSizeCalculator;
REGISTER_CALCULATOR(EndLoopImageSizeCalculator);

}  // namespace mediapipe
