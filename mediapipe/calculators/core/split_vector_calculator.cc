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

#include "mediapipe/calculators/core/split_vector_calculator.h"

#include <vector>

#include "mediapipe/framework/formats/classification.pb.h"
#include "mediapipe/framework/formats/detection.pb.h"
#include "mediapipe/framework/formats/image.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/matrix.h"
#include "mediapipe/framework/formats/rect.pb.h"
#include "mediapipe/framework/formats/tensor.h"
#include "tensorflow/lite/interpreter.h"

#if !defined(MEDIAPIPE_DISABLE_GL_COMPUTE)
#include "tensorflow/lite/delegates/gpu/gl/gl_buffer.h"
#endif  // !defined(MEDIAPIPE_DISABLE_GL_COMPUTE)

namespace mediapipe {

// Example config:
// node {
//   calculator: "SplitTfLiteTensorVectorCalculator"
//   input_stream: "tflitetensor_vector"
//   output_stream: "tflitetensor_vector_range_0"
//   output_stream: "tflitetensor_vector_range_1"
//   options {
//     [mediapipe.SplitVectorCalculatorOptions.ext] {
//       ranges: { begin: 0 end: 1 }
//       ranges: { begin: 1 end: 2 }
//       element_only: false
//     }
//   }
// }
typedef SplitVectorCalculator<TfLiteTensor, false>
    SplitTfLiteTensorVectorCalculator;
REGISTER_CALCULATOR(SplitTfLiteTensorVectorCalculator);

typedef SplitVectorCalculator<Tensor, true> SplitTensorVectorCalculator;
REGISTER_CALCULATOR(SplitTensorVectorCalculator);

typedef SplitVectorCalculator<mediapipe::NormalizedLandmark, false>
    SplitLandmarkVectorCalculator;
REGISTER_CALCULATOR(SplitLandmarkVectorCalculator);

typedef SplitVectorCalculator<mediapipe::NormalizedLandmarkList, false>
    SplitNormalizedLandmarkListVectorCalculator;
REGISTER_CALCULATOR(SplitNormalizedLandmarkListVectorCalculator);

typedef SplitVectorCalculator<mediapipe::NormalizedRect, false>
    SplitNormalizedRectVectorCalculator;
REGISTER_CALCULATOR(SplitNormalizedRectVectorCalculator);

typedef SplitVectorCalculator<Matrix, false> SplitMatrixVectorCalculator;
REGISTER_CALCULATOR(SplitMatrixVectorCalculator);

#if !defined(MEDIAPIPE_DISABLE_GL_COMPUTE)
typedef SplitVectorCalculator<::tflite::gpu::gl::GlBuffer, true>
    MovableSplitGlBufferVectorCalculator;
REGISTER_CALCULATOR(MovableSplitGlBufferVectorCalculator);
#endif

typedef SplitVectorCalculator<mediapipe::Detection, false>
    SplitDetectionVectorCalculator;
REGISTER_CALCULATOR(SplitDetectionVectorCalculator);

typedef SplitVectorCalculator<mediapipe::ClassificationList, false>
    SplitClassificationListVectorCalculator;
REGISTER_CALCULATOR(SplitClassificationListVectorCalculator);

typedef SplitVectorCalculator<uint64_t, false> SplitUint64tVectorCalculator;
REGISTER_CALCULATOR(SplitUint64tVectorCalculator);

typedef SplitVectorCalculator<float, false> SplitFloatVectorCalculator;
REGISTER_CALCULATOR(SplitFloatVectorCalculator);

typedef SplitVectorCalculator<mediapipe::Image, false>
    SplitImageVectorCalculator;
REGISTER_CALCULATOR(SplitImageVectorCalculator);

typedef SplitVectorCalculator<std::array<float, 16>, false>
    SplitAffineMatrixVectorCalculator;
REGISTER_CALCULATOR(SplitAffineMatrixVectorCalculator);

}  // namespace mediapipe
