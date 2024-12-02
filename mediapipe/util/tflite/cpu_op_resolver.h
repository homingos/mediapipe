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

#ifndef MEDIAPIPE_UTIL_TFLITE_CPU_OP_RESOLVER_H_
#define MEDIAPIPE_UTIL_TFLITE_CPU_OP_RESOLVER_H_

#include "tensorflow/lite/kernels/register.h"

namespace mediapipe {

// This function registers the CPU implementations for following custom ops:
// "Convolution2DTransposeBias"
// "MaxPoolArgmax"
// "MaxUnpooling"
extern "C" void MediaPipe_RegisterTfLiteOpResolver(tflite::MutableOpResolver*);

// This resolver is used for the custom ops introduced by
// `MediaPipe_RegisterTfLiteOpResolver` (see above).
class CpuOpResolver
    : public tflite::ops::builtin::BuiltinOpResolverWithoutDefaultDelegates {
 public:
  CpuOpResolver() { MediaPipe_RegisterTfLiteOpResolver(this); }
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_UTIL_TFLITE_CPU_OP_RESOLVER_H_
