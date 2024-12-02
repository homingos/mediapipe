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
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/port/status_builder.h"

namespace mediapipe {

// Takes in a string, draws the text string by cv::putText(), and outputs an
// ImageFrame.
//
// Example config:
// node {
//   calculator: "OpenCvPutTextCalculator"
//   input_stream: "text_to_put"
//   output_stream: "out_image_frames"
// }
// TODO: Generalize the calculator for other text use cases.
class OpenCvPutTextCalculator : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc);
  absl::Status Process(CalculatorContext* cc) override;
};

absl::Status OpenCvPutTextCalculator::GetContract(CalculatorContract* cc) {
  cc->Inputs().Index(0).Set<std::string>();
  cc->Outputs().Index(0).Set<ImageFrame>();
  return absl::OkStatus();
}

absl::Status OpenCvPutTextCalculator::Process(CalculatorContext* cc) {
  const std::string& text_content = cc->Inputs().Index(0).Get<std::string>();
  cv::Mat mat = cv::Mat::zeros(640, 640, CV_8UC4);
  cv::putText(mat, text_content, cv::Point(15, 70), cv::FONT_HERSHEY_PLAIN, 3,
              cv::Scalar(255, 255, 0, 255), 4);
  std::unique_ptr<ImageFrame> output_frame = absl::make_unique<ImageFrame>(
      ImageFormat::SRGBA, mat.size().width, mat.size().height);
  mat.copyTo(formats::MatView(output_frame.get()));
  cc->Outputs().Index(0).Add(output_frame.release(), cc->InputTimestamp());
  return absl::OkStatus();
}

REGISTER_CALCULATOR(OpenCvPutTextCalculator);

}  // namespace mediapipe
