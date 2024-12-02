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

#ifndef MEDIAPIPE_UTIL_TFLITE_OPERATIONS_TRANSPOSE_CONV_BIAS_H_
#define MEDIAPIPE_UTIL_TFLITE_OPERATIONS_TRANSPOSE_CONV_BIAS_H_

#include "tensorflow/lite/kernels/internal/types.h"
#include "tensorflow/lite/kernels/kernel_util.h"

namespace mediapipe {
namespace tflite_operations {

TfLiteRegistration* RegisterConvolution2DTransposeBias();

}  // namespace tflite_operations
}  // namespace mediapipe

#endif  // MEDIAPIPE_UTIL_TFLITE_OPERATIONS_TRANSPOSE_CONV_BIAS_H_
