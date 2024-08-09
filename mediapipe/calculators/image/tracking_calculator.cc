#include <vector>

#include "mediapipe/calculators/image/feature_detector_calculator.pb.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/port/opencv_core_inc.h"
#include "mediapipe/framework/port/opencv_video_inc.h"
#include "mediapipe/framework/port/opencv_features2d_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/opencv_calib3d_inc.h"
#include "mediapipe/framework/tool/options_util.h"
#include "mediapipe/framework/port/threadpool.h"
#include "absl/synchronization/blocking_counter.h"
#include "mediapipe/util/tracking/box_tracker.pb.h"
#include "mediapipe/util/tracking/box_tracker.h"
#include "absl/status/status.h"
#include <omp.h>
namespace mediapipe
{

    const char kOptionsTag[] = "OPTIONS";
    const int kPatchSize = 32;
    const int kNumThreads = 2;
    const double MAX_CHANGE_THRESHOLD = 70.0;
    // Calculator to find homography and warp a secondary image onto the primary
    // image.
    class TrackingCalculator : public CalculatorBase
    {
    public:
        static absl::Status GetContract(CalculatorContract *cc)
        {
            cc->Inputs().Tag("PRIMARY_IMAGE").Set<ImageFrame>();
            cc->Inputs().Tag("SECONDARY_IMAGE").Set<ImageFrame>();
            cc->Inputs().Tag("FEEDBACK_I").Set<cv::Mat>();
            cc->Outputs().Tag("OVERLAYED_IMAGE").Set<ImageFrame>();
            cc->Outputs().Tag("FEEDBACK_O").Set<cv::Mat>();
            cc->Outputs().Tag("BOXES").Set<TimedBoxProtoList>();
            return absl::OkStatus();
        }

        absl::Status Open(CalculatorContext *cc) override
        {
            cc->SetOffset(::mediapipe::TimestampDiff(0));
            feature_detector_ = cv::ORB::create(
                500, 1.4, 8, kPatchSize - 1, 0, 4, cv::ORB::FAST_SCORE);
            matcher_ = cv::makePtr<cv::FlannBasedMatcher>(cv::makePtr<cv::flann::LshIndexParams>(20, 10, 2));
            pool_ = absl::make_unique<mediapipe::ThreadPool>("ThreadPool", kNumThreads);
            pool_->StartWorkers();
            return absl::OkStatus();
        }

        absl::Status Process(CalculatorContext *cc) override
        {
            if (!cc->Inputs().Tag("FEEDBACK_I").IsEmpty())
            {
                return absl::OkStatus();
            }
            // Handle the first non-empty packet from SECONDARY_IMAGE
            if (!cc->Inputs().Tag("SECONDARY_IMAGE").IsEmpty())
            {
                if (cc->Inputs().Tag("PRIMARY_IMAGE").IsEmpty())
                {
                    ABSL_LOG(INFO) << "PRIMARY_IMAGE is empty";
                    return absl::OkStatus();
                }
                const auto &secondary_image =
                    cc->Inputs().Tag("SECONDARY_IMAGE").Get<ImageFrame>();
                const auto &primary_image =
                    cc->Inputs().Tag("PRIMARY_IMAGE").Get<ImageFrame>();
                // Process and copy the SECONDARY_IMAGE packet to LOOP_IMAGE
                return ProcessAndCopy(cc, primary_image, secondary_image);
            }

            // Check if PRIMARY_IMAGE is empty
            if (cc->Inputs().Tag("PRIMARY_IMAGE").IsEmpty())
            {
                // If PRIMARY_IMAGE is empty, send an empty ImageFrame to
                // OVERLAYED_IMAGE
                cv::Mat image(640, 480, CV_8UC3, cv::Scalar(255, 255, 255));
                std::unique_ptr<ImageFrame> output_image_frame =
                    absl::make_unique<ImageFrame>(ImageFormat::SRGB, image.cols,
                                                  image.rows,
                                                  ImageFrame::kGlDefaultAlignmentBoundary);
                image.copyTo(formats::MatView(output_image_frame.get()));
                cc->Outputs()
                    .Tag("OVERLAYED_IMAGE")
                    .Add(output_image_frame.release(), cc->InputTimestamp());
                return absl::OkStatus();
            }

            const auto &primary_image =
                cc->Inputs().Tag("PRIMARY_IMAGE").Get<ImageFrame>();

            cc->Outputs().Tag("OVERLAYED_IMAGE").AddPacket(cc->Inputs().Tag("PRIMARY_IMAGE").Value());
            return absl::OkStatus();
        }

    private:
        absl::Status ProcessAndCopy(CalculatorContext *cc,
                                    const ImageFrame &primary_image,
                                    const ImageFrame &secondary_image)
        {
            // Process the overlay
            OverlayImages(cc, primary_image, secondary_image);
            return absl::OkStatus();
        }

        absl::Status ProcessOverlay(CalculatorContext *cc,
                                    const ImageFrame &primary_image,
                                    const ImageFrame &loop_image)
        {
            // Process the overlay
            OverlayImages(cc, primary_image, loop_image);
            return absl::OkStatus();
        }

        void OverlayImages(CalculatorContext *cc,
                           const ImageFrame &primary_image,
                           const ImageFrame &secondary_image)
        {
            try
            {
                cv::Mat primary_view = formats::MatView(&primary_image);
                cv::Mat secondary_view = formats::MatView(&secondary_image);
                // Convert to grayscale for feature detection.
                cv::Mat primary_gray, secondary_gray;

                // Detect ORB features.
                std::vector<cv::KeyPoint> primary_keypoints, secondary_keypoints;
                cv::Mat primary_descriptors, secondary_descriptors;
                absl::BlockingCounter counter(2);
                pool_->Schedule(
                    [this, &primary_view, &primary_gray, &primary_keypoints, &primary_descriptors, &counter]
                    {
                        cv::cvtColor(primary_view, primary_gray, cv::COLOR_BGR2GRAY);
                        feature_detector_->detectAndCompute(
                            primary_gray, cv::noArray(), primary_keypoints, primary_descriptors);
                        counter.DecrementCount();
                    });
                pool_->Schedule(
                    [this, &secondary_view, &secondary_gray, &secondary_keypoints, &secondary_descriptors, &counter]
                    {
                        cv::cvtColor(secondary_view, secondary_gray, cv::COLOR_BGR2GRAY);
                        feature_detector_->detectAndCompute(
                            secondary_gray, cv::noArray(), secondary_keypoints, secondary_descriptors);
                        counter.DecrementCount();
                    });
                counter.Wait();

                // Match features using brute force matcher.
                std::vector<std::vector<cv::DMatch>> knn_matches;
                try
                {
                    if (primary_descriptors.empty() || secondary_descriptors.empty())
                    {
                        std::unique_ptr<ImageFrame> output_image_frame =
                            absl::make_unique<ImageFrame>(ImageFormat::SRGB, 0,
                                                          0, ImageFrame::kGlDefaultAlignmentBoundary);
                        cc->Outputs()
                            .Tag("OVERLAYED_IMAGE")
                            .Add(output_image_frame.release(), cc->InputTimestamp());
                        return;
                    }

                    matcher_->knnMatch(primary_descriptors, secondary_descriptors,
                                       knn_matches, 2);
                }
                catch (const cv::Exception &e)
                {
                    LOG(ERROR) << "OpenCV Exception during matching: " << e.what();
                    cc->Outputs()
                        .Tag("OVERLAYED_IMAGE")
                        .AddPacket(cc->Inputs().Tag("PRIMARY_IMAGE").Value());
                    return;
                }

                const float ratio_thresh = 0.7f;
                std::vector<cv::DMatch> good_matches;

#pragma omp parallel for num_threads(16)
                for (size_t i = 0; i < knn_matches.size(); i++)
                {
                    if (knn_matches[i][0].distance <
                        ratio_thresh * knn_matches[i][1].distance)
                    {
                        good_matches.push_back(knn_matches[i][0]);
                    }
                }

                // Find homography matrix.
                std::vector<cv::Point2f> primary_points, secondary_points;

#pragma omp parallel for num_threads(16)
                for (const auto &match : good_matches)
                {
                    primary_points.push_back(primary_keypoints[match.queryIdx].pt);
                    secondary_points.push_back(secondary_keypoints[match.trainIdx].pt);
                }

                cv::Mat homography;
                if (primary_points.size() >= 16 && secondary_points.size() >= 16)
                {
                    ABSL_LOG(INFO) << "Primary Points :" << primary_points.size();
                    ABSL_LOG(INFO) << "Secondary Points" << secondary_points.size();
                    homography = cv::findHomography(secondary_points, primary_points, cv::RANSAC);
                    if (!cc->Inputs().Tag("FEEDBACK_I").IsEmpty() && !homography.empty())
                    {
                        const cv::Mat &previousHomography = cc->Inputs().Tag("FEEDBACK_I").Get<cv::Mat>();
                        if (!previousHomography.empty())
                        {
                            cv::Mat diff = homography - previousHomography;
                            double change = cv::norm(diff);

                            if (change > MAX_CHANGE_THRESHOLD)
                            {
                                // If change is too large, use a weighted average
                                double weight = MAX_CHANGE_THRESHOLD / change;
                                homography = weight * homography + (1 - weight) * previousHomography;
                            }
                        }
                    }
                }
                else
                {
                    // LOG(WARNING) << "Insufficient keypoints for homography. Sending primary"
                    //              << " image as is.";
                    // cc->Outputs()
                    //     .Tag("OVERLAYED_IMAGE")
                    //     .AddPacket(cc->Inputs().Tag("PRIMARY_IMAGE").Value());
                    return;
                }

                // Warp the secondary image based on the homography.
                cv::Mat warped_secondary_image;
                if (!homography.empty())
                {
                    cv::cvtColor(primary_view, primary_gray, cv::COLOR_BGR2GRAY);
                    cv::warpPerspective(secondary_view, warped_secondary_image, homography,
                                        primary_view.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, 0);

                    const int templateWidth = 400;
                    const int templateHeight = 200;

                    // Define template corners (in template image coordinates)
                    std::vector<cv::Point2f> templateCorners = {
                        cv::Point2f(0, 0),                          // Top-left
                        cv::Point2f(templateWidth, 0),              // Top-right
                        cv::Point2f(templateWidth, templateHeight), // Bottom-right
                        cv::Point2f(0, templateHeight)              // Bottom-left
                    };

                    std::vector<cv::Point2f> projectedCorners;
                    cv::perspectiveTransform(templateCorners, projectedCorners, homography);

                    // Construct TimedBoxProtoList
                    TimedBoxProtoList timed_box_proto_list;
                    auto *box_ptr = timed_box_proto_list.add_box();

                    // Assuming you want to track only one box (the template)
                    box_ptr->set_id(0); // Assign a unique ID

                    // Add vertices to TimedBoxProto in COUNTER-CLOCKWISE order:
                    box_ptr->mutable_quad()->add_vertices(projectedCorners[0].x / primary_view.cols);
                    box_ptr->mutable_quad()->add_vertices(projectedCorners[0].y / primary_view.rows);

                    box_ptr->mutable_quad()->add_vertices(projectedCorners[3].x / primary_view.cols);
                    box_ptr->mutable_quad()->add_vertices(projectedCorners[3].y / primary_view.rows);

                    box_ptr->mutable_quad()->add_vertices(projectedCorners[2].x / primary_view.cols);
                    box_ptr->mutable_quad()->add_vertices(projectedCorners[2].y / primary_view.rows);

                    box_ptr->mutable_quad()->add_vertices(projectedCorners[1].x / primary_view.cols);
                    box_ptr->mutable_quad()->add_vertices(projectedCorners[1].y / primary_view.rows);

                    std::unique_ptr<TimedBoxProtoList> output_boxes(new TimedBoxProtoList());
                    *output_boxes = std::move(timed_box_proto_list);
                    cc->Outputs().Tag("BOXES").Add(output_boxes.release(), cc->InputTimestamp());
                }
                else
                {
                    LOG(ERROR) << "Homography matrix is empty. Sending primary image as is.";
                    std::unique_ptr<ImageFrame> output_image_frame =
                        absl::make_unique<ImageFrame>(ImageFormat::SRGB, 0,
                                                      0, ImageFrame::kGlDefaultAlignmentBoundary);
                    cc->Outputs()
                        .Tag("OVERLAYED_IMAGE")
                        .Add(output_image_frame.release(), cc->InputTimestamp());
                    return;
                }

                // Overlay the warped secondary image onto the primary image.
                cv::Mat overlayed_image;
                if (!warped_secondary_image.empty())
                {
                    // TODO: OVerlay wrapped image on primary image
                    overlayed_image = warped_secondary_image.clone();
                }
                else
                {
                    overlayed_image = primary_view.clone(); // Just return the primary image if
                                                            // no homography is found
                }

                // Create an ImageFrame for the overlaid image.
                std::unique_ptr<ImageFrame> output_image_frame =
                    absl::make_unique<ImageFrame>(ImageFormat::SRGB, overlayed_image.cols,
                                                  overlayed_image.rows, ImageFrame::kGlDefaultAlignmentBoundary);
                overlayed_image.copyTo(formats::MatView(output_image_frame.get()));

                // Send the output packet.
                auto feedback_homography = absl::make_unique<cv::Mat>(homography);
                cc->Outputs()
                    .Tag("FEEDBACK_O")
                    .Add(feedback_homography.release(), cc->InputTimestamp());
                cc->Outputs()
                    .Tag("OVERLAYED_IMAGE")
                    .Add(output_image_frame.release(), cc->InputTimestamp());
                return;
            }
            catch (const std::exception &e)
            {
                ABSL_LOG(ERROR) << "An error occurred: " << e.what();
                return;
            }
        }

        cv::Ptr<cv::Feature2D> feature_detector_;
        cv::Ptr<cv::FlannBasedMatcher> matcher_;
        std::unique_ptr<mediapipe::ThreadPool> pool_;
    };

    REGISTER_CALCULATOR(TrackingCalculator);

} // namespace mediapipe