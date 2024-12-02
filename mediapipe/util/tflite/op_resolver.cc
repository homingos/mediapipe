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

#include "mediapipe/util/tflite/op_resolver.h"

#include "tensorflow/lite/builtin_ops.h"
#include "tensorflow/lite/c/builtin_op_data.h"
#include "tensorflow/lite/c/c_api.h"
#include "tensorflow/lite/c/c_api_opaque.h"

namespace mediapipe {
namespace {

constexpr char kMaxPoolingWithArgmax2DOpName[] = "MaxPoolingWithArgmax2D";
constexpr int kMaxPoolingWithArgmax2DOpVersion = 1;

constexpr char kMaxUnpooling2DOpName[] = "MaxUnpooling2D";
constexpr int kMaxUnpooling2DOpVersion = 1;

constexpr char kConvolution2DTransposeBiasOpName[] =
    "Convolution2DTransposeBias";
constexpr int kConvolution2DTransposeBiasOpVersion = 1;

TfLiteRegistration* RegisterMaxPoolingWithArgmax2D() {
  static TfLiteOperator* reg_external = []() {
    // Intentionally allocated and never destroyed.
    auto* r = TfLiteOperatorCreate(kTfLiteBuiltinCustom,
                                   kMaxPoolingWithArgmax2DOpName,
                                   kMaxPoolingWithArgmax2DOpVersion);
    TfLiteOperatorSetInit(
        r, [](TfLiteOpaqueContext*, const char*, size_t) -> void* {
          return new TfLitePaddingValues();
        });
    TfLiteOperatorSetFree(r, [](TfLiteOpaqueContext*, void* buffer) -> void {
      delete reinterpret_cast<TfLitePaddingValues*>(buffer);
    });
    TfLiteOperatorSetPrepare(
        r,
        [](TfLiteOpaqueContext* context,
           TfLiteOpaqueNode* node) -> TfLiteStatus { return kTfLiteOk; });
    TfLiteOperatorSetInvoke(
        r, [](TfLiteOpaqueContext* context, TfLiteOpaqueNode*) -> TfLiteStatus {
          TfLiteOpaqueContextReportError(
              context, "MaxPoolingWithArgmax2D is only available on the GPU.");
          return kTfLiteError;
        });
    return r;
  }();
  static TfLiteRegistration reg{};
  reg.registration_external = reg_external;
  return &reg;
}

TfLiteRegistration* RegisterMaxUnpooling2D() {
  static TfLiteOperator* reg_external =
      // Intentionally allocated and never destroyed.
      TfLiteOperatorCreate(kTfLiteBuiltinCustom, kMaxUnpooling2DOpName,
                           kMaxUnpooling2DOpVersion);
  static TfLiteRegistration reg{};
  reg.registration_external = reg_external;
  return &reg;
}

TfLiteRegistration* RegisterConvolution2DTransposeBias() {
  static TfLiteOperator* reg_external =
      // Intentionally allocated and never destroyed.
      TfLiteOperatorCreate(kTfLiteBuiltinCustom,
                           kConvolution2DTransposeBiasOpName,
                           kConvolution2DTransposeBiasOpVersion);
  static TfLiteRegistration reg{};
  reg.registration_external = reg_external;
  return &reg;
}

}  // namespace

OpResolver::OpResolver() {
  AddCustom(kMaxPoolingWithArgmax2DOpName, RegisterMaxPoolingWithArgmax2D(),
            kMaxPoolingWithArgmax2DOpVersion);
  AddCustom(kMaxUnpooling2DOpName, RegisterMaxUnpooling2D(),
            kMaxUnpooling2DOpVersion);
  AddCustom(kConvolution2DTransposeBiasOpName,
            RegisterConvolution2DTransposeBias(),
            kConvolution2DTransposeBiasOpVersion);
}

}  // namespace mediapipe
