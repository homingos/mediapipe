#include <vector>
#include <algorithm>
#include <thread>
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
  const int kGridSize = 1; // 4x4 grid
  const int kNumCells = kGridSize * kGridSize;
  constexpr size_t kCacheLineSize = 64;

  class FeatureDetection : public CalculatorBase
  {
  public:
    static absl::Status GetContract(CalculatorContract *cc)
    {
      cc->Inputs().Tag("IMAGE").Set<ImageFrame>();
      cc->Outputs().Tag("FEATURES").Set<cv::Mat>();
      return absl::OkStatus();
    }

    absl::Status Open(CalculatorContext *cc) override
    {
      options_ = tool::RetrieveOptions(cc->Options(), cc->InputSidePackets(),
                                       kOptionsTag)
                     .GetExtension(FeatureDetectorCalculatorOptions::ext);

      // Initialize feature detector with optimized parameters
      constexpr int thresh = 10;
      constexpr int octaves = 6;
      constexpr float patternScale = 0.85f;
      feature_detector_ = cv::BRISK::create(thresh, octaves, patternScale);

      // Pre-allocate buffers for each grid cell
      cell_buffers_.resize(kNumCells);

      cc->SetOffset(TimestampDiff(0));
      return absl::OkStatus();
    }

    absl::Status Process(CalculatorContext *cc) override
    {
      if (cc->InputTimestamp() == Timestamp::PreStream())
      {
        return absl::OkStatus();
      }

      // Get input image
      const auto &input_frame = cc->Inputs().Tag("IMAGE").Get<ImageFrame>();
      cv::Mat input_view = formats::MatView(&input_frame);

      // Convert to grayscale
      cv::Mat grayscale;
      cv::cvtColor(input_view, grayscale, cv::COLOR_RGB2GRAY, 1);

      // Calculate grid cell dimensions
      const int cell_height = grayscale.rows / kGridSize;
      const int cell_width = grayscale.cols / kGridSize;

      // Process each grid cell in parallel
      std::vector<std::vector<cv::KeyPoint>> grid_keypoints(kNumCells);
      std::vector<cv::Mat> grid_descriptors(kNumCells);

      #pragma omp parallel for collapse(kGridSize)
      for (int i = 0; i < kGridSize; ++i)
      {
        #pragma omp parallel for collapse(kGridSize)
        for (int j = 0; j < kGridSize; ++j)
        {
          const int cell_idx = i * kGridSize + j;

          // Define cell ROI
          cv::Rect roi(
              j * cell_width,
              i * cell_height,
              (j == kGridSize - 1) ? grayscale.cols - j * cell_width : cell_width,
              (i == kGridSize - 1) ? grayscale.rows - i * cell_height : cell_height);

          // Extract cell ROI
          cv::Mat cell = grayscale(roi);

          // Detect features in cell
          std::vector<cv::KeyPoint> cell_keypoints;
          cv::Mat cell_descriptors;
          feature_detector_->detectAndCompute(
              cell, cv::noArray(),
              cell_keypoints, cell_descriptors);

          // Adjust keypoint coordinates to global image space
          for (auto &kp : cell_keypoints)
          {
            kp.pt.x += roi.x;
            kp.pt.y += roi.y;
          }

          // Store results
          grid_keypoints[cell_idx] = std::move(cell_keypoints);
          grid_descriptors[cell_idx] = std::move(cell_descriptors);
        }
      }

      // Merge results from all cells
      std::vector<cv::KeyPoint> all_keypoints;
      std::vector<cv::Mat> valid_descriptors;

      // Calculate total number of keypoints for pre-allocation
      size_t total_keypoints = 0;
      for (const auto &kps : grid_keypoints)
      {
        total_keypoints += kps.size();
      }
      all_keypoints.reserve(total_keypoints);

      // Merge keypoints and collect valid descriptors
      for (size_t i = 0; i < kNumCells; ++i)
      {
        all_keypoints.insert(
            all_keypoints.end(),
            grid_keypoints[i].begin(),
            grid_keypoints[i].end());

        if (!grid_descriptors[i].empty())
        {
          valid_descriptors.push_back(grid_descriptors[i]);
        }
      }

      // Combine descriptors
      cv::Mat combined_descriptors;
      if (!valid_descriptors.empty())
      {
        cv::vconcat(valid_descriptors, combined_descriptors);
      }

      // Apply feature limit if specified
      if (options_.has_max_features() &&
          all_keypoints.size() > options_.max_features())
      {
        selectTopFeatures(all_keypoints, combined_descriptors,
                          options_.max_features());
      }

      // Output results
      auto output_descriptors = std::make_unique<cv::Mat>();
      combined_descriptors.copyTo(*output_descriptors);
      cc->Outputs().Tag("FEATURES").Add(output_descriptors.release(), cc->InputTimestamp());

      return absl::OkStatus();
    }

  private:
    FeatureDetectorCalculatorOptions options_;
    cv::Ptr<cv::Feature2D> feature_detector_;
    std::vector<cv::Mat> cell_buffers_;

    void selectTopFeatures(std::vector<cv::KeyPoint> &keypoints,
                           cv::Mat &descriptors,
                           int max_features)
    {
      // Sort keypoints by response
      std::vector<size_t> indices(keypoints.size());
      std::iota(indices.begin(), indices.end(), 0);

      std::partial_sort(
          indices.begin(),
          indices.begin() + max_features,
          indices.end(),
          [&](size_t i1, size_t i2)
          {
            return keypoints[i1].response > keypoints[i2].response;
          });

      // Keep only top features
      std::vector<cv::KeyPoint> top_keypoints;
      cv::Mat top_descriptors(max_features, descriptors.cols, descriptors.type());

      top_keypoints.reserve(max_features);
      for (int i = 0; i < max_features; ++i)
      {
        top_keypoints.push_back(keypoints[indices[i]]);
        descriptors.row(indices[i]).copyTo(top_descriptors.row(i));
      }

      keypoints = std::move(top_keypoints);
      descriptors = top_descriptors;
    }
  };

  REGISTER_CALCULATOR(FeatureDetection);

} // namespace mediapipe