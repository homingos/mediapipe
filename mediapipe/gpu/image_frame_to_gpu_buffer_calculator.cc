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

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/gpu/gl_calculator_helper.h"

#ifdef __APPLE__
#include "mediapipe/objc/util.h"
#endif

namespace mediapipe {

// Convert ImageFrame to GpuBuffer.
//
// NOTE: all ImageFrameToGpuBufferCalculators use a common dedicated shared GL
// context thread by default, which is different from the main GL context thread
// used by the graph. (If MediaPipe uses multithreading and multiple OpenGL
// contexts.)
class ImageFrameToGpuBufferCalculator : public CalculatorBase {
 public:
  ImageFrameToGpuBufferCalculator() {}

  static absl::Status GetContract(CalculatorContract* cc);

  absl::Status Open(CalculatorContext* cc) override;
  absl::Status Process(CalculatorContext* cc) override;

 private:
#if !MEDIAPIPE_GPU_BUFFER_USE_CV_PIXEL_BUFFER
  GlCalculatorHelper helper_;
#endif  // !MEDIAPIPE_GPU_BUFFER_USE_CV_PIXEL_BUFFER
};
REGISTER_CALCULATOR(ImageFrameToGpuBufferCalculator);

// static
absl::Status ImageFrameToGpuBufferCalculator::GetContract(
    CalculatorContract* cc) {
  cc->Inputs().Index(0).Set<ImageFrame>();
  cc->Outputs().Index(0).Set<GpuBuffer>();
  // Note: we call this method even on platforms where we don't use the helper,
  // to ensure the calculator's contract is the same. In particular, the helper
  // enables support for the legacy side packet, which several graphs still use.
  MP_RETURN_IF_ERROR(GlCalculatorHelper::UpdateContract(cc));
  return absl::OkStatus();
}

absl::Status ImageFrameToGpuBufferCalculator::Open(CalculatorContext* cc) {
  // Inform the framework that we always output at the same timestamp
  // as we receive a packet at.
  cc->SetOffset(TimestampDiff(0));
#if !MEDIAPIPE_GPU_BUFFER_USE_CV_PIXEL_BUFFER
  MP_RETURN_IF_ERROR(helper_.Open(cc));
#endif  // !MEDIAPIPE_GPU_BUFFER_USE_CV_PIXEL_BUFFER
  return absl::OkStatus();
}

absl::Status ImageFrameToGpuBufferCalculator::Process(CalculatorContext* cc) {
#if MEDIAPIPE_GPU_BUFFER_USE_CV_PIXEL_BUFFER
  CFHolder<CVPixelBufferRef> buffer;
  MP_RETURN_IF_ERROR(CreateCVPixelBufferForImageFramePacket(
      cc->Inputs().Index(0).Value(), &buffer));
  cc->Outputs().Index(0).Add(new GpuBuffer(buffer), cc->InputTimestamp());
#else
  const auto& input = cc->Inputs().Index(0).Get<ImageFrame>();
  helper_.RunInGlContext([this, &input, &cc]() {
    GlTexture dst = helper_.CreateDestinationTexture(input);
    std::unique_ptr<GpuBuffer> output = dst.GetFrame<GpuBuffer>();
    cc->Outputs().Index(0).Add(output.release(), cc->InputTimestamp());
    dst.Release();
  });
#endif  // MEDIAPIPE_GPU_BUFFER_USE_CV_PIXEL_BUFFER
  return absl::OkStatus();
}

}  // namespace mediapipe
