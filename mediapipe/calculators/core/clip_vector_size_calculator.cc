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

#include "mediapipe/calculators/core/clip_vector_size_calculator.h"

#include <vector>

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/detection.pb.h"
#include "mediapipe/framework/formats/rect.pb.h"

namespace mediapipe {

typedef ClipVectorSizeCalculator<::mediapipe::NormalizedRect>
    ClipNormalizedRectVectorSizeCalculator;
REGISTER_CALCULATOR(ClipNormalizedRectVectorSizeCalculator);

typedef ClipVectorSizeCalculator<::mediapipe::Detection>
    ClipDetectionVectorSizeCalculator;
REGISTER_CALCULATOR(ClipDetectionVectorSizeCalculator);

}  // namespace mediapipe
