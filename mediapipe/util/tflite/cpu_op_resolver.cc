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

#include "mediapipe/util/tflite/cpu_op_resolver.h"

#include "absl/log/absl_check.h"
#include "mediapipe/framework/port/logging.h"
#include "mediapipe/util/tflite/operations/landmarks_to_transform_matrix.h"
#include "mediapipe/util/tflite/operations/max_pool_argmax.h"
#include "mediapipe/util/tflite/operations/max_unpooling.h"
#include "mediapipe/util/tflite/operations/resampler.h"
#include "mediapipe/util/tflite/operations/transform_landmarks.h"
#include "mediapipe/util/tflite/operations/transform_tensor_bilinear.h"
#include "mediapipe/util/tflite/operations/transpose_conv_bias.h"
#include "tensorflow/lite/builtin_op_data.h"
#include "tensorflow/lite/mutable_op_resolver.h"

namespace mediapipe {

void MediaPipe_RegisterTfLiteOpResolver(tflite::MutableOpResolver *resolver) {
  ABSL_CHECK(resolver != nullptr);
  resolver->AddCustom("MaxPoolingWithArgmax2D",
                      tflite_operations::RegisterMaxPoolingWithArgmax2D());
  resolver->AddCustom("MaxUnpooling2D",
                      tflite_operations::RegisterMaxUnpooling2D());
  resolver->AddCustom("Convolution2DTransposeBias",
                      tflite_operations::RegisterConvolution2DTransposeBias());

  resolver->AddCustom("TransformTensorBilinear",
                      tflite_operations::RegisterTransformTensorBilinearV2(),
                      /*version=*/2);
  resolver->AddCustom("TransformLandmarks",
                      tflite_operations::RegisterTransformLandmarksV2(),
                      /*version=*/2);
  resolver->AddCustom("Landmarks2TransformMatrix",
                      tflite_operations::RegisterLandmarksToTransformMatrixV2(),
                      /*version=*/2);

  resolver->AddCustom("Resampler", tflite_operations::RegisterResampler(),
                      /*version=*/1);
}

}  // namespace mediapipe
