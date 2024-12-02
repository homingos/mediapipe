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
            cc->Outputs().Tag("TRACKING").Set<int>();
            return absl::OkStatus();
        }

        absl::Status Open(CalculatorContext *cc) override
        {
            cc->SetOffset(::mediapipe::TimestampDiff(0));
            tracking_lost = false;
            tracking_lost_count = 0;
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
                std::vector<float> output_boxes;
                // Input validation
                if (box_list.box_size() > 0 && input_props.first > 0 && input_props.second > 0 && box_list.box(0).confidence() == 1)
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
                    std::array<Vector2_f, 4> corners = currentBox.Corners(input_props.second, input_props.first);

                    // Create and send output
                    const float scale_factor = -2.02f;
                    const float x_offset = 1.01f;
                    const float y_offset = 1.01f;
                    output_boxes.push_back(((scale_factor* corners[0].y() /  (input_props.first))) + y_offset);
                    output_boxes.push_back(((scale_factor* corners[0].x() /  (input_props.second))) + x_offset);
                    output_boxes.push_back(0.0f);
                    output_boxes.push_back(((scale_factor* corners[3].y() /  (input_props.first))) + y_offset);
                    output_boxes.push_back(((scale_factor* corners[3].x() /  (input_props.second))) + x_offset);
                    output_boxes.push_back(0.0f);
                    output_boxes.push_back(((scale_factor* corners[1].y() /  (input_props.first))) + y_offset);
                    output_boxes.push_back(((scale_factor* corners[1].x() /  (input_props.second))) + x_offset);
                    output_boxes.push_back(0.0f);
                    output_boxes.push_back(((scale_factor* corners[2].y() /  (input_props.first))) + y_offset);
                    output_boxes.push_back(((scale_factor* corners[2].x() /  (input_props.second))) + x_offset);
                    output_boxes.push_back(0.0f);

                    // Reset tracking lost count when we have valid tracking
                    tracking_lost_count = 0;
                    if(tracking_lost){
                        tracking_lost = false;
                    }
                }
                else
                {
                    output_boxes.insert(output_boxes.end(), {0.0f, 0.0f, 0.0f, 0.0f, 
                                       0.0f, 0.0f, 0.0f, 0.0f,
                                       0.0f, 0.0f, 0.0f, 0.0f});
                    
                    // Increment tracking lost count
                    tracking_lost_count++;
                    
                    // Only set tracking_lost to true if we've lost tracking for more than 20 frames
                    if(!tracking_lost && tracking_lost_count > 5){
                        tracking_lost_count = 0;
                        tracking_lost = true;
                    }
                }
                cc->Outputs().Tag("TRACKING").Add(absl::make_unique<int>(tracking_lost).release(), cc->InputTimestamp());
                auto output_floats = absl::make_unique<std::vector<float>>(output_boxes);
                cc->Outputs().Tag("BFLOATS").Add(output_floats.release(), cc->InputTimestamp());
            }
            catch (const std::exception &e)
            {
                ABSL_LOG(ERROR) << "Exception: " << e.what();
            }

            return absl::OkStatus();
        }

    private:
        bool tracking_lost;
        int tracking_lost_count;  // Added counter for tracking lost frames
    };

    REGISTER_CALCULATOR(BoxToFloatsCalculator);

} // namespace mediapipe