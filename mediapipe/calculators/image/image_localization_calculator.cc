#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/port/opencv_core_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe
{

    class PictureLocalizationCalculator : public CalculatorBase
    {
    public:
        static absl::Status GetContract(CalculatorContract *cc)
        {
            cc->Inputs().Tag("IMAGE").Set<ImageFrame>();
            cc->Outputs().Tag("IMAGE").Set<ImageFrame>();
            return absl::OkStatus();
        }

        absl::Status Open(CalculatorContext *cc) override
        {
            cc->SetOffset(TimestampDiff(0));
            return absl::OkStatus();
        }

        absl::Status Process(CalculatorContext *cc) override
        {
            const auto &input_img = cc->Inputs().Tag("IMAGE").Get<ImageFrame>();
            cv::Mat image = formats::MatView(&input_img);
            cv::Mat cropped_image = LocalizePicture(image);

            std::unique_ptr<ImageFrame> output_frame(
                new ImageFrame(input_img.Format(), cropped_image.cols, cropped_image.rows));
            cropped_image.copyTo(formats::MatView(output_frame.get()));
            cc->Outputs().Tag("IMAGE").Add(output_frame.release(), cc->InputTimestamp());
            return absl::OkStatus();
        }

    private:
        cv::Mat LocalizePicture(const cv::Mat &image)
        {
            cv::Mat gray, blur, edges;
            cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

            // Apply Gaussian blur to reduce noise
            cv::GaussianBlur(gray, blur, cv::Size(5, 5), 0);

            // Use Canny edge detection
            cv::Canny(blur, edges, 50, 150);

            // Dilate edges to close gaps
            cv::Mat dilated_edges;
            cv::dilate(edges, dilated_edges, cv::Mat(), cv::Point(-1, -1), 2);

            std::vector<std::vector<cv::Point>> contours;
            cv::findContours(dilated_edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

            if (contours.empty())
            {
                return image;
            }

            // Find the contour with the largest area
            auto max_contour = std::max_element(contours.begin(), contours.end(),
                                                [](const std::vector<cv::Point> &c1, const std::vector<cv::Point> &c2)
                                                {
                                                    return cv::contourArea(c1) < cv::contourArea(c2);
                                                });

            // Check if the largest contour is significant enough
            double max_area = cv::contourArea(*max_contour);
            if (max_area < 0.10 * image.cols * image.rows)
            {
                return image; // Return original if no significant contour found
            }

            // Approximate the contour to a polygon
            std::vector<cv::Point> approx;
            cv::approxPolyDP(*max_contour, approx, 0.02 * cv::arcLength(*max_contour, true), true);

            // If we have a quadrilateral, perform perspective transform
            if (approx.size() == 4)
            {
                cv::Point2f src_pts[4], dst_pts[4];
                for (int i = 0; i < 4; i++)
                {
                    src_pts[i] = cv::Point2f(approx[i].x, approx[i].y);
                }

                // Sort points to ensure consistent ordering
                std::sort(src_pts, src_pts + 4, [](const cv::Point2f &a, const cv::Point2f &b)
                          { return a.x + a.y < b.x + b.y; });

                // Define destination points for a rectangle
                cv::Rect bounds = cv::boundingRect(approx);
                dst_pts[0] = cv::Point2f(0, 0);
                dst_pts[1] = cv::Point2f(bounds.width - 1, 0);
                dst_pts[2] = cv::Point2f(bounds.width - 1, bounds.height - 1);
                dst_pts[3] = cv::Point2f(0, bounds.height - 1);

                cv::Mat transform = cv::getPerspectiveTransform(src_pts, dst_pts);
                cv::Mat result;
                cv::warpPerspective(image, result, transform, bounds.size());
                return result;
            }
            else
            {
                // If not a quadrilateral, just return the bounding rectangle
                return image(cv::boundingRect(approx));
            }
        }
    };

    REGISTER_CALCULATOR(PictureLocalizationCalculator);

} // namespace mediapipe