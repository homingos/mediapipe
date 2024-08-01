#include <vector>

#include "mediapipe/calculators/image/feature_detector_calculator.pb.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/port/opencv_core_inc.h"
#include "mediapipe/framework/port/opencv_features2d_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/opencv_calib3d_inc.h"
#include "mediapipe/framework/tool/options_util.h"
#include "absl/status/status.h"

namespace mediapipe
{

    const char kOptionsTag[] = "OPTIONS";
    const int kPatchSize = 32;
    const int kNumThreads = 16;

    // Calculator to find homography and warp a secondary image onto the primary
    // image.
    class ImageOverlayCalculator : public CalculatorBase
    {
    public:
        static absl::Status GetContract(CalculatorContract *cc)
        {
            cc->Inputs().Tag("PRIMARY_IMAGE").Set<ImageFrame>();
            cc->Inputs().Tag("SECONDARY_IMAGE").Set<ImageFrame>();
            cc->Inputs().Tag("LOOP_IMAGE").Set<ImageFrame>();
            cc->Outputs().Tag("OVERLAYED_IMAGE").Set<ImageFrame>();
            cc->Outputs().Tag("SECONDARY_IMAGE_LOOP").Set<ImageFrame>();
            return absl::OkStatus();
        }

        absl::Status Open(CalculatorContext *cc) override
        {
            cc->SetOffset(TimestampDiff(0));
            feature_detector_ = cv::ORB::create(
                200,
                1.2,
                4,
                kPatchSize - 1, 0, 2, cv::ORB::FAST_SCORE);
            matcher_ = cv::makePtr<cv::FlannBasedMatcher>(cv::makePtr<cv::flann::LshIndexParams>(12, 20, 2));
            return absl::OkStatus();
        }

        absl::Status Process(CalculatorContext *cc) override
        {
            // Check if PRIMARY_IMAGE is empty
            if (cc->Inputs().Tag("PRIMARY_IMAGE").IsEmpty())
            {
                // If PRIMARY_IMAGE is empty, send an empty ImageFrame to OVERLAYED_IMAGE
                auto black_frame = absl::make_unique<ImageFrame>(
                    ImageFormat::SRGB, 640, 480, ImageFrame::kGlDefaultAlignmentBoundary);
                cv::Mat black_view = formats::MatView(black_frame.get());
                black_view.setTo(cv::Scalar(0, 0, 0)); // Set all pixels to black (BGR)
                cc->Outputs().Tag("OVERLAYED_IMAGE").Add(black_frame.release(), cc->InputTimestamp());
                return absl::OkStatus();
            }

            const auto &primary_image = cc->Inputs().Tag("PRIMARY_IMAGE").Get<ImageFrame>();

            // Handle the first non-empty packet from SECONDARY_IMAGE
            if (!cc->Inputs().Tag("SECONDARY_IMAGE").IsEmpty())
            {
                const auto &secondary_image =
                    cc->Inputs().Tag("SECONDARY_IMAGE").Get<ImageFrame>();

                // Process and copy the SECONDARY_IMAGE packet to LOOP_IMAGE
                ProcessAndCopy(cc, primary_image, secondary_image);
                return absl::OkStatus();
            }

            // Handle subsequent packets, using the LOOP_IMAGE
            if (!cc->Inputs().Tag("LOOP_IMAGE").IsEmpty())
            {
                // const auto &loop_image =
                //     cc->Inputs().Tag("LOOP_IMAGE").Get<ImageFrame>();

                // // Process the PRIMARY_IMAGE with the LOOP_IMAGE overlay
                // ProcessOverlay(cc, primary_image, loop_image);
                // return absl::OkStatus();
                cc->Outputs().Tag("OVERLAYED_IMAGE").AddPacket(cc->Inputs().Tag("PRIMARY_IMAGE").Value());
                return absl::OkStatus();
            }

            // Both SECONDARY_IMAGE and LOOP_IMAGE are empty, just pass the primary image
            cc->Outputs().Tag("OVERLAYED_IMAGE").AddPacket(cc->Inputs().Tag("PRIMARY_IMAGE").Value());
            return absl::OkStatus();
        }

    private:
        void ProcessAndCopy(CalculatorContext *cc,
                            const ImageFrame &primary_image,
                            const ImageFrame &secondary_image)
        {
            // Process the overlay
            OverlayImages(cc, primary_image, secondary_image);

            // Copy SECONDARY_IMAGE to LOOP_IMAGE
            cc->Outputs().Tag("SECONDARY_IMAGE_LOOP").AddPacket(cc->Inputs().Tag("SECONDARY_IMAGE").Value());
        }

        void ProcessOverlay(CalculatorContext *cc,
                            const ImageFrame &primary_image,
                            const ImageFrame &loop_image)
        {
            // Process the overlay
            OverlayImages(cc, primary_image, loop_image);
        }

        absl::Status OverlayImages(CalculatorContext *cc,
                                   const ImageFrame &primary_image,
                                   const ImageFrame &secondary_image)
        {

            cv::Mat primary_view = formats::MatView(&primary_image);
            cv::Mat secondary_view = formats::MatView(&secondary_image);

            // Convert to grayscale for feature detection.
            cv::Mat primary_gray, secondary_gray;
            cv::cvtColor(primary_view, primary_gray, cv::COLOR_BGR2GRAY);
            cv::cvtColor(secondary_view, secondary_gray, cv::COLOR_BGR2GRAY);

            // Detect ORB features.
            std::vector<cv::KeyPoint> primary_keypoints, secondary_keypoints;
            cv::Mat primary_descriptors, secondary_descriptors;
            feature_detector_->detectAndCompute(
                primary_gray, cv::noArray(), primary_keypoints, primary_descriptors);
            feature_detector_->detectAndCompute(
                secondary_gray, cv::noArray(), secondary_keypoints, secondary_descriptors);

            // Match features using brute force matcher.
            std::vector<std::vector<cv::DMatch>> knn_matches;
            try
            {
                if (primary_descriptors.empty() || secondary_descriptors.empty())
                {
                    LOG(WARNING) << "Empty descriptors. Skipping matching.";
                    cc->Outputs().Tag("OVERLAYED_IMAGE").AddPacket(cc->Inputs().Tag("PRIMARY_IMAGE").Value());
                    return absl::OkStatus();
                }

                if (primary_descriptors.type() != CV_32F || secondary_descriptors.type() != CV_32F)
                {
                    LOG(ERROR) << "Descriptor type mismatch!";
                    return absl::InternalError("Descriptor type mismatch!");
                }

                matcher_->knnMatch(primary_descriptors, secondary_descriptors, knn_matches, 2);
            }
            catch (const cv::Exception &e)
            {
                cc->Outputs().Tag("OVERLAYED_IMAGE").AddPacket(cc->Inputs().Tag("PRIMARY_IMAGE").Value());
                return absl::OkStatus();
            }

            const float ratio_thresh = 0.7f;
            std::vector<cv::DMatch> good_matches;
            for (size_t i = 0; i < knn_matches.size(); i++)
            {
                if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
                {
                    good_matches.push_back(knn_matches[i][0]);
                }
            }

            // Find homography matrix.
            std::vector<cv::Point2f> primary_points, secondary_points;
            for (const auto &match : good_matches)
            {
                primary_points.push_back(primary_keypoints[match.queryIdx].pt);
                secondary_points.push_back(secondary_keypoints[match.trainIdx].pt);
            }
            cv::Mat homography;
            if (primary_points.size() >= 4 && secondary_points.size() >= 4)
            {
                homography = cv::findHomography(secondary_points, primary_points, cv::RANSAC);
            }
            else
            {
                cc->Outputs().Tag("OVERLAYED_IMAGE").AddPacket(cc->Inputs().Tag("PRIMARY_IMAGE").Value());
                return absl::OkStatus();
            }

            // Warp the secondary image based on the homography.
            cv::Mat warped_secondary_image;
            if (!homography.empty())
            {
                cv::warpPerspective(secondary_view, warped_secondary_image, homography, primary_view.size());
            }

            // Overlay the warped secondary image onto the primary image.
            cv::Mat overlayed_image;
            if (!warped_secondary_image.empty())
            {
                overlayed_image = primary_view.clone(); // Create a copy of the primary image.
                // You can use different blending modes here, e.g.,
                // cv::addWeighted(overlayed_image, 0.5, warped_secondary_image, 0.5, 0, overlayed_image); // Blend with 50% transparency
                overlayed_image.copyTo(overlayed_image, warped_secondary_image); // Simple copy (overwrites primary image where secondary exists)
            }
            else
            {
                overlayed_image = primary_view.clone(); // Just return the primary image if no homography is found
            }

            // Create an ImageFrame for the overlaid image.
            std::unique_ptr<ImageFrame> output_image_frame = absl::make_unique<ImageFrame>(
                ImageFormat::SRGB, overlayed_image.cols, overlayed_image.rows, ImageFrame::kGlDefaultAlignmentBoundary);
            overlayed_image.copyTo(formats::MatView(output_image_frame.get()));

            // Send the output packet.
            cc->Outputs()
                .Tag("OVERLAYED_IMAGE")
                .Add(output_image_frame.release(), cc->InputTimestamp());
            cc->Outputs().Tag("SECONDARY_IMAGE_LOOP").AddPacket(cc->Inputs().Tag("LOOP_IMAGE").Value());
            return absl::OkStatus();
        }

        cv::Ptr<cv::Feature2D>
            feature_detector_;
        cv::Ptr<cv::FlannBasedMatcher> matcher_;
    };

    REGISTER_CALCULATOR(ImageOverlayCalculator);

} // namespace mediapipe