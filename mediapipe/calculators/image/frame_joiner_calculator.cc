#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/port/opencv_core_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/tool/options_util.h"

namespace mediapipe
{

    class FrameJoinerCalculator : public CalculatorBase
    {
    public:
        static absl::Status GetContract(CalculatorContract *cc)
        {
            // cc->SetInputStreamHandler("ImmediateInputStreamHandler");
            if (cc->Inputs().HasTag("FRAME"))
            {
                cc->Inputs().Tag("FRAME").Set<ImageFrame>();
            }
            if (cc->Inputs().HasTag("OVERLAY"))
            {
                cc->Inputs().Tag("OVERLAY").Set<ImageFrame>();
            }
            if (cc->Outputs().HasTag("COMBINED"))
            {
                cc->Outputs().Tag("COMBINED").Set<ImageFrame>();
            }

            return absl::OkStatus();
        }

        absl::Status Open(CalculatorContext *cc) override
        {
            cc->SetOffset(::mediapipe::TimestampDiff(0));
            return absl::OkStatus();
        }

        absl::Status Process(CalculatorContext *cc) override
        {
            if (!cc->Inputs().Tag("OVERLAY").IsEmpty())
            {
                cv::Mat overlayed_image;
                const auto &IFFrame = cc->Inputs().Tag("FRAME").Get<ImageFrame>();
                const auto &IFOverlay = cc->Inputs().Tag("OVERLAY").Get<ImageFrame>();
                cv::Mat frame = formats::MatView(&IFFrame);
                cv::Mat overlay = formats::MatView(&IFOverlay);
                if (overlay.cols == 0)
                {
                    cc->Outputs()
                        .Tag("COMBINED")
                        .AddPacket(cc->Inputs().Tag("FRAME").Value());
                    return absl::OkStatus();
                }
                cv::cvtColor(frame, frame, cv::COLOR_BGRA2BGR);
                cv::cvtColor(overlay, overlay, cv::COLOR_BGRA2BGR);
                cv::addWeighted(frame, 1.0, overlay, 1.0, 0, overlayed_image);
                std::unique_ptr<ImageFrame> output_image_frame =
                    absl::make_unique<ImageFrame>(ImageFormat::SRGB, overlayed_image.cols,
                                                  overlayed_image.rows, ImageFrame::kGlDefaultAlignmentBoundary);
                overlayed_image.copyTo(formats::MatView(output_image_frame.get()));
                cc->Outputs()
                    .Tag("COMBINED")
                    .Add(output_image_frame.release(), cc->InputTimestamp());
                return absl::OkStatus();
            }
            if (!cc->Inputs().Tag("FRAME").IsEmpty())
            {
                cc->Outputs().Tag("COMBINED").AddPacket(cc->Inputs().Tag("FRAME").Value());
                return absl::OkStatus();
            }

            return absl::OkStatus();
        }
    };
    REGISTER_CALCULATOR(FrameJoinerCalculator);
} // namespace mediapipe