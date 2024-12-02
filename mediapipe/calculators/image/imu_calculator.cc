#include <vector>
#include "mediapipe/framework/calculator_framework.h"
#include "absl/status/status.h"

namespace mediapipe {

class ImuTransformCalculator : public CalculatorBase {
public:
    static absl::Status GetContract(CalculatorContract* cc) {
        cc->Inputs().Tag("BOX_FLOATS").Set<std::vector<float>>();  // Input from BoxToFloatsCalculator
        cc->Inputs().Tag("IMU_ROTATION").Set<std::vector<float>>(); // Rotation matrix from IMU
        cc->Outputs().Tag("TRANSFORMED_FLOATS").Set<std::vector<float>>();
        return absl::OkStatus();
    }

    absl::Status Process(CalculatorContext* cc) override {
        if (cc->Inputs().Tag("BOX_FLOATS").IsEmpty() || 
            cc->Inputs().Tag("IMU_ROTATION").IsEmpty()) {
            return absl::OkStatus();
        }

        try {
            const auto& box_floats = cc->Inputs().Tag("BOX_FLOATS").Get<std::vector<float>>();
            const auto& rotation = cc->Inputs().Tag("IMU_ROTATION").Get<std::vector<float>>();

            // Validate inputs
            if (box_floats.size() != 12 || rotation.size() != 9) {  // 4 corners × 3 coordinates
                std::vector<float> default_output(12, 0.0f);
                auto output = absl::make_unique<std::vector<float>>(default_output);
                cc->Outputs().Tag("TRANSFORMED_FLOATS").Add(output.release(), cc->InputTimestamp());
                return absl::OkStatus();
            }

            std::vector<float> transformed_floats(12);
            
            // Process each corner while maintaining order
            // Corner order: bottom-left(0), bottom-right(3), top-right(1), top-left(2)
            const int corner_indices[] = {0, 3, 1, 2};  // Maps to the correct corner order
            
            for (int i = 0; i < 4; i++) {
                int src_idx = corner_indices[i] * 3;  // Source index in box_floats
                int dst_idx = i * 3;                  // Destination index in transformed_floats
                
                // Get input coordinates
                float x = box_floats[src_idx];
                float y = box_floats[src_idx + 1];
                float z = box_floats[src_idx + 2];

                // Apply rotation matrix
                // Note: We're using the IMU rotation to adjust the points in screen space
                transformed_floats[dst_idx] = rotation[0] * x + rotation[1] * y + rotation[2] * z;
                transformed_floats[dst_idx + 1] = rotation[3] * x + rotation[4] * y + rotation[5] * z;
                transformed_floats[dst_idx + 2] = rotation[6] * x + rotation[7] * y + rotation[8] * z;
            }

            auto output_floats = absl::make_unique<std::vector<float>>(transformed_floats);
            cc->Outputs().Tag("TRANSFORMED_FLOATS").Add(
                output_floats.release(), cc->InputTimestamp());

        } catch (const std::exception& e) {
            ABSL_LOG(ERROR) << "Exception in ImuTransformCalculator: " << e.what();
            // Output zero-filled vector in case of error
            std::vector<float> error_output(12, 0.0f);
            auto output = absl::make_unique<std::vector<float>>(error_output);
            cc->Outputs().Tag("TRANSFORMED_FLOATS").Add(output.release(), cc->InputTimestamp());
        }

        return absl::OkStatus();
    }
};

REGISTER_CALCULATOR(ImuTransformCalculator);

}  // namespace mediapipe