// Copyright 2023 mediapipe.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/port/opencv_core_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe
{

    // Finds the largest contour within a given image based on HSV color
    // thresholds. The largest contour is assumed to be the picture. If a valid
    // contour is found, the image will be cropped to the contour's bounding
    // box.
    //
    // Inputs:
    //   IMAGE: An ImageFrame containing the input image.
    //
    // Outputs:
    //   IMAGE: An ImageFrame containing the cropped image.
    //
    // Example config:
    //   node {
    //     calculator: "PictureLocalizationCalculator"
    //     input_stream: "IMAGE:input_image"
    //     output_stream: "IMAGE:cropped_image"
    //   }
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

            // Create output ImageFrame.
            std::unique_ptr<ImageFrame> output_frame(
                new ImageFrame(input_img.Format(), cropped_image.cols, cropped_image.rows));
            cropped_image.copyTo(formats::MatView(output_frame.get()));
            cc->Outputs().Tag("IMAGE").Add(output_frame.release(),
                                           cc->InputTimestamp());
            return absl::OkStatus();
        }
        absl::Status Close(CalculatorContext *cc)
        {
            return absl::OkStatus();
        }

    private:
        cv::Mat LocalizePicture(const cv::Mat &image)
        {
            // Convert to HSV color space
            cv::Mat hsv;
            cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

            // Define thresholds for picture colors
            cv::Scalar lower_picture(10, 10, 10);    // Lower HSV bound
            cv::Scalar upper_picture(110, 110, 110); // Upper HSV bound
            int min_bbox_size = 1000;

            // Create a mask for the picture
            cv::Mat mask;
            cv::inRange(hsv, lower_picture, upper_picture, mask);

            // Find contours in the mask
            std::vector<std::vector<cv::Point>> contours;
            cv::findContours(mask, contours, cv::RETR_CCOMP,
                             cv::CHAIN_APPROX_SIMPLE);

            // Find the largest contour
            int max_area = 0;
            int max_index = -1;
            for (int i = 0; i < contours.size(); ++i)
            {
                int area = cv::contourArea(contours[i]);
                if (area > max_area)
                {
                    max_area = area;
                    max_index = i;
                }
            }

            // Crop the bounding box
            if (max_index >= 0)
            {
                cv::Rect rect = cv::boundingRect(contours[max_index]);
                if (rect.width * rect.height > min_bbox_size)
                {
                    return image(rect);
                }
            }
            return image; // Return original image if no suitable contour is found
        }
    };

    REGISTER_CALCULATOR(PictureLocalizationCalculator);

} // namespace mediapipe