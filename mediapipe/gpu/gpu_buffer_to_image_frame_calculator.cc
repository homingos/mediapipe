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
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/port/status.h"

#define HAVE_GPU_BUFFER
#ifdef __APPLE__
#include "mediapipe/objc/util.h"
#endif

#include "mediapipe/gpu/gl_calculator_helper.h"

namespace mediapipe {

// Convert an input image (GpuBuffer or ImageFrame) to ImageFrame.
//
// NOTE: all GpuBufferToImageFrameCalculators use a common dedicated shared GL
// context thread by default, which is different from the main GL context thread
// used by the graph. (If MediaPipe uses multithreading and multiple OpenGL
// contexts.)
//
// IMPORTANT: graph writer must make sure input GpuBuffer backed OpenGL texture
// is not in use before the calculator starts processing and not used by any
// other code until the calculator returns:
// - pixel transfer involves attaching GpuBuffer backing texture as a logical
//   buffer to a particular bound framebuffer.
// - and if texture is already bound and enabled for texturing, this may lead
//   to a "feedback loop" and undefined results.
// See, OpenGL ES 3.0 Spec 4.4.3 "Feedback Loops between Textures and the
// Framebuffer"
//
class GpuBufferToImageFrameCalculator : public CalculatorBase {
 public:
  GpuBufferToImageFrameCalculator() {}

  static absl::Status GetContract(CalculatorContract* cc);

  absl::Status Open(CalculatorContext* cc) override;
  absl::Status Process(CalculatorContext* cc) override;

 private:
#if !MEDIAPIPE_GPU_BUFFER_USE_CV_PIXEL_BUFFER
  GlCalculatorHelper helper_;
#endif  // MEDIAPIPE_GPU_BUFFER_USE_CV_PIXEL_BUFFER
};
REGISTER_CALCULATOR(GpuBufferToImageFrameCalculator);

// static
absl::Status GpuBufferToImageFrameCalculator::GetContract(
    CalculatorContract* cc) {
  cc->Inputs().Index(0).SetAny();
  cc->Outputs().Index(0).Set<ImageFrame>();
  // Note: we call this method even on platforms where we don't use the helper,
  // to ensure the calculator's contract is the same. In particular, the helper
  // enables support for the legacy side packet, which several graphs still use.
  MP_RETURN_IF_ERROR(GlCalculatorHelper::UpdateContract(cc));
  return absl::OkStatus();
}

absl::Status GpuBufferToImageFrameCalculator::Open(CalculatorContext* cc) {
  // Inform the framework that we always output at the same timestamp
  // as we receive a packet at.
  cc->SetOffset(TimestampDiff(0));
#if !MEDIAPIPE_GPU_BUFFER_USE_CV_PIXEL_BUFFER
  MP_RETURN_IF_ERROR(helper_.Open(cc));
#endif  // MEDIAPIPE_GPU_BUFFER_USE_CV_PIXEL_BUFFER
  return absl::OkStatus();
}

absl::Status GpuBufferToImageFrameCalculator::Process(CalculatorContext* cc) {
  if (cc->Inputs().Index(0).Value().ValidateAsType<ImageFrame>().ok()) {
    cc->Outputs().Index(0).AddPacket(cc->Inputs().Index(0).Value());
    return absl::OkStatus();
  }

#ifdef HAVE_GPU_BUFFER
  if (cc->Inputs().Index(0).Value().ValidateAsType<GpuBuffer>().ok()) {
    const auto& input = cc->Inputs().Index(0).Get<GpuBuffer>();
#if MEDIAPIPE_GPU_BUFFER_USE_CV_PIXEL_BUFFER
    std::unique_ptr<ImageFrame> frame =
        CreateImageFrameForCVPixelBuffer(GetCVPixelBufferRef(input));
    cc->Outputs().Index(0).Add(frame.release(), cc->InputTimestamp());
#else
    helper_.RunInGlContext([this, &input, &cc]() {
      auto src = helper_.CreateSourceTexture(input);
      std::unique_ptr<ImageFrame> frame = absl::make_unique<ImageFrame>(
          ImageFormatForGpuBufferFormat(input.format()), src.width(),
          src.height(), ImageFrame::kGlDefaultAlignmentBoundary);
      helper_.BindFramebuffer(src);
      const auto info = GlTextureInfoForGpuBufferFormat(input.format(), 0,
                                                        helper_.GetGlVersion());
      glReadPixels(0, 0, src.width(), src.height(), info.gl_format,
                   info.gl_type, frame->MutablePixelData());
      cc->Outputs().Index(0).Add(frame.release(), cc->InputTimestamp());
      src.Release();
    });
#endif  // MEDIAPIPE_GPU_BUFFER_USE_CV_PIXEL_BUFFER
    return absl::OkStatus();
  }
#endif  // defined(HAVE_GPU_BUFFER)

  return absl::Status(absl::StatusCode::kInvalidArgument,
                      "Input packets must be ImageFrame or GpuBuffer.");
}

}  // namespace mediapipe
