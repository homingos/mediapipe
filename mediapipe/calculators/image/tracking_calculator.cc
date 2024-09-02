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
#include "absl/synchronization/mutex.h"
#include "mediapipe/util/tracking/parallel_invoker.h"
#include "absl/status/status.h"
#include <omp.h>
namespace mediapipe
{

    const char kOptionsTag[] = "OPTIONS";
    const int kPatchSize = 32;
    const int kNumThreads = 2;
    const double MAX_CHANGE_THRESHOLD = 1.14;
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
            cc->Outputs().Tag("FEEDBACK_O").Set<cv::Mat>();
            cc->Outputs().Tag("BOXES").Set<TimedBoxProtoList>();
            return absl::OkStatus();
        }

        absl::Status Open(CalculatorContext *cc) override
        {
            cc->SetOffset(::mediapipe::TimestampDiff(0));

            feature_detector_ = cv::AKAZE::create(cv::AKAZE::DESCRIPTOR_MLDB, 200);
            matcher_ = cv::makePtr<cv::FlannBasedMatcher>(cv::makePtr<cv::flann::LshIndexParams>(20, 10, 2));
            pool_ = absl::make_unique<mediapipe::ThreadPool>("TrackingPool", kNumThreads);
            pool_->StartWorkers();
            match_found_ = false;
            dets = 0;
            return absl::OkStatus();
        }

        absl::Status Process(CalculatorContext *cc) override
        {

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
                return absl::OkStatus();
            }

            return absl::OkStatus();
        }

    private:
        cv::Ptr<cv::Feature2D>
            feature_detector_;
        cv::Ptr<cv::FlannBasedMatcher>
            matcher_;
        std::unique_ptr<mediapipe::ThreadPool> pool_;
        absl::Status ProcessAndCopy(CalculatorContext *cc,
                                    const ImageFrame &primary_image,
                                    const ImageFrame &secondary_image)
        {
            // Process the overlay
            OverlayImages(cc, primary_image, secondary_image);
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
                        return;
                    }

                    matcher_->knnMatch(primary_descriptors, secondary_descriptors,
                                       knn_matches, 2);
                }
                catch (const cv::Exception &e)
                {
                    LOG(ERROR) << "OpenCV Exception during matching: " << e.what();
                    return;
                }

                const float ratio_thresh = 0.75f;
                std::vector<cv::DMatch> good_matches;
                ParallelFor(0, knn_matches.size(), 1,
                            [&knn_matches, &good_matches, ratio_thresh](const BlockedRange &b)
                            {
                                for (size_t i = b.begin(); i != b.end(); ++i)
                                {
                                    if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
                                    {
                                        good_matches.push_back(knn_matches[i][0]);
                                    }
                                }
                            });

                // Find homography matrix.
                std::vector<cv::Point2f> primary_points, secondary_points;
                ParallelFor(0, good_matches.size(), 1,
                            [&good_matches, &primary_keypoints, &secondary_keypoints,
                             &primary_points, &secondary_points](const BlockedRange &b)
                            {
                                for (size_t i = b.begin(); i != b.end(); ++i)
                                {
                                    const auto &match = good_matches[i];
                                    primary_points.push_back(primary_keypoints[match.queryIdx].pt);
                                    secondary_points.push_back(secondary_keypoints[match.trainIdx].pt);
                                }
                            });

                // Estimate fundamental matrix
                cv::Mat fundamental_matrix = cv::findFundamentalMat(primary_points, secondary_points, cv::FM_RANSAC);

                // Filter matches using epipolar constraint
                std::vector<cv::DMatch> epipolar_matches;
                std::vector<cv::Point2f> filtered_primary_points, filtered_secondary_points;
                const double epipolar_thresh = 1.0; // Adjust this threshold as needed

                for (size_t i = 0; i < primary_points.size(); ++i)
                {
                    cv::Mat point1 = (cv::Mat_<double>(3, 1) << primary_points[i].x, primary_points[i].y, 1);
                    cv::Mat point2 = (cv::Mat_<double>(3, 1) << secondary_points[i].x, secondary_points[i].y, 1);

                    // Calculate epipolar line
                    cv::Mat epiline = fundamental_matrix * point1;

                    // Calculate distance to epipolar line
                    double a = epiline.at<double>(0);
                    double b = epiline.at<double>(1);
                    double c = epiline.at<double>(2);
                    double dist = std::abs(a * point2.at<double>(0) + b * point2.at<double>(1) + c) / std::sqrt(a * a + b * b);

                    if (dist < epipolar_thresh)
                    {
                        epipolar_matches.push_back(good_matches[i]);
                        filtered_primary_points.push_back(primary_points[i]);
                        filtered_secondary_points.push_back(secondary_points[i]);
                    }
                }

                // Find homography matrix using filtered matches
                cv::Mat homography;
                if (filtered_primary_points.size() >= 8 && filtered_secondary_points.size() >= 8)
                {
                    homography = cv::findHomography(filtered_secondary_points, filtered_primary_points, cv::RANSAC);
                    if (!homography.empty())
                    {
                        // Define template corners (in template image coordinates)
                        std::vector<cv::Point2f> templateCorners = {
                            cv::Point2f(0, 0),                                     // Top-left
                            cv::Point2f(secondary_view.cols, 0),                   // Top-right
                            cv::Point2f(secondary_view.cols, secondary_view.rows), // Bottom-right
                            cv::Point2f(0, secondary_view.rows)                    // Bottom-left
                        };

                        std::vector<cv::Point2f> projectedCorners;
                        cv::perspectiveTransform(templateCorners, projectedCorners, homography);

                        // Construct TimedBoxProtoList
                        TimedBoxProtoList timed_box_proto_list;
                        auto *box_ptr = timed_box_proto_list.add_box();

                        // Assuming you want to track only one box (the template)
                        box_ptr->set_id(420); // Assign a unique ID
                        box_ptr->set_reacquisition(true);
                        box_ptr->set_aspect_ratio(primary_view.cols / primary_view.rows);
                        auto ts = cc->InputTimestamp();
                        box_ptr->set_time_msec(ts.Microseconds() / 1000);

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
                        // Send the output packet.
                        match_found_ = true;
                        dets++;
                        auto feedback_homography = absl::make_unique<cv::Mat>(homography);
                        cc->Outputs()
                            .Tag("FEEDBACK_O")
                            .Add(feedback_homography.release(), cc->InputTimestamp());
                        return;
                    }
                }else{
                    return;
                }
            }
            catch (const std::exception &e)
            {
                ABSL_LOG(ERROR) << "An error occurred: " << e.what();
                return;
            }
        }

        bool match_found_;
        int dets;
    };
    REGISTER_CALCULATOR(TrackingCalculator);

} // namespace mediapipe