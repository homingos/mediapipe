#include <vector>

#include "mediapipe/calculators/image/feature_detector_calculator.pb.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/port/opencv_core_inc.h"
#include "mediapipe/framework/port/opencv_features2d_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/tool/options_util.h"
#include "absl/status/status.h"

namespace mediapipe
{

  const char kOptionsTag[] = "OPTIONS";
  const int kPatchSize = 32;
  const int kNumThreads = 16;

  class FeatureDetection : public CalculatorBase
  {
  public:
    static absl::Status GetContract(CalculatorContract *cc)
    {
      if (cc->Inputs().HasTag("IMAGE"))
      {
        cc->Inputs().Tag("IMAGE").Set<ImageFrame>();
      }
      if (cc->Outputs().HasTag("FEATURES"))
      {
        cc->Outputs().Tag("FEATURES").Set<cv::Mat>();
      }
      return absl::OkStatus();
    }

    absl::Status Open(CalculatorContext *cc) override
    {
      options_ =
          tool::RetrieveOptions(cc->Options(), cc->InputSidePackets(), kOptionsTag)
              .GetExtension(FeatureDetectorCalculatorOptions::ext);
      feature_detector_ = cv::ORB::create(
          options_.max_features(), options_.scale_factor(),
          options_.pyramid_level(), kPatchSize - 1, 0, 2, cv::ORB::FAST_SCORE);
      return absl::OkStatus();
    }

    absl::Status Process(CalculatorContext *cc) override
    {
      const Timestamp &timestamp = cc->InputTimestamp();
      if (timestamp == Timestamp::PreStream())
      {
        // Indicator packet.
        return absl::OkStatus();
      }
      InputStream *input_frame = &(cc->Inputs().Tag("IMAGE"));
      cv::Mat input_view = formats::MatView(&input_frame->Get<ImageFrame>());
      cv::Mat grayscale_view;
      cv::cvtColor(input_view, grayscale_view, cv::COLOR_RGB2GRAY);

      std::vector<cv::KeyPoint> keypoints;
      cv::Mat descriptors;
      feature_detector_->detectAndCompute(grayscale_view, cv::noArray(), keypoints,
                                          descriptors);

      // Resize descriptors if necessary
      if (keypoints.size() < options_.max_features())
      {
        descriptors.resize(keypoints.size());
      }

      // Output descriptors
      auto descriptors_ptr = absl::make_unique<cv::Mat>(descriptors);
      cc->Outputs().Tag("FEATURES").Add(descriptors_ptr.release(), timestamp);

      return absl::OkStatus();
    }

  private:
    FeatureDetectorCalculatorOptions options_;
    cv::Ptr<cv::Feature2D> feature_detector_;
  };

  REGISTER_CALCULATOR(FeatureDetection);

} // namespace mediapipe