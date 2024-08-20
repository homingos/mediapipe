#include <vector>

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/util/tracking/box_tracker.pb.h"
#include "mediapipe/util/tracking/box_tracker.h"
#include "absl/status/status.h"

namespace mediapipe
{
    class BoxToFloatsCalculator : public CalculatorBase
    {
    public:
        static absl::Status GetContract(CalculatorContract *cc)
        {
            cc->Inputs().Tag("BOXES").Set<TimedBoxProtoList>();
            cc->Inputs().Tag("PROPS").Set<std::pair<int, int>>();
            cc->Outputs().Tag("BFLOATS").Set<std::vector<float>>();
            return absl::OkStatus();
        }

        absl::Status Open(CalculatorContext *cc) override
        {
            cc->SetOffset(::mediapipe::TimestampDiff(0));
            return absl::OkStatus();
        }

        absl::Status Process(CalculatorContext *cc) override
        {
            if (cc->Inputs().Tag("BOXES").IsEmpty() || cc->Inputs().Tag("PROPS").IsEmpty())
            {
                return absl::OkStatus();
            }
            try
            {
                const auto &box_list = cc->Inputs().Tag("BOXES").Get<TimedBoxProtoList>();
                const auto &input_props = cc->Inputs().Tag("PROPS").Get<std::pair<int, int>>();

                // Input validation
                if (box_list.box_size() > 0 && input_props.first > 0 && input_props.second > 0)
                {
                    // Convert TimedBoxProto to TimedBox
                    TimedBox currentBox;
                    try
                    {
                        currentBox = TimedBox::FromProto(box_list.box(0));
                    }
                    catch (const std::exception &e)
                    {
                        ABSL_LOG(ERROR) << "Error converting TimedBoxProto: " << e.what();
                        // Handle the error (e.g., skip this box or send an empty vector)
                        return absl::OkStatus();
                    }

                    // Calculate box corners
                    std::array<Vector2_f, 4> corners = currentBox.Corners(input_props.first, input_props.second);

                    // Create and send output
                    std::vector<float> output_boxes;
                    for (const auto &corner : corners)
                    {
                        output_boxes.push_back(corner.x());
                        output_boxes.push_back(corner.y());
                    }
                    auto output_floats = absl::make_unique<std::vector<float>>(output_boxes);
                    cc->Outputs().Tag("BFLOATS").Add(output_floats.release(), cc->InputTimestamp());
                }
                else
                {
                    // Handle the case where input data is invalid
                    // (e.g., send an empty vector or log a message).
                }
            }
            catch (const std::exception &e)
            {
                ABSL_LOG(ERROR) << "Exception: " << e.what();
            }

            return absl::OkStatus();
        }
    };

    REGISTER_CALCULATOR(BoxToFloatsCalculator);

} // namespace mediapipe