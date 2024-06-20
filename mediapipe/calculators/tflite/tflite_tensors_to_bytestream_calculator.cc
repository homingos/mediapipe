// Copyright 2019 The MediaPipe Authors.
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
#include "mediapipe/framework/port/ret_check.h"
#include "tensorflow/lite/interpreter.h"
#include <sstream>

namespace mediapipe {

constexpr char kTensorsTag[] = "TENSORS";
constexpr char kBytesTag[] = "BYTES";

// A calculator for converting TFLite tensors to a byte stream.
//
// Input:
//  TENSORS - Vector of TfLiteTensor of type kTfLiteFloat32. Only the first
//            tensor will be used.
// Output:
//  BYTES - Converted byte stream.
//
// Usage example:
// node {
//   calculator: "TfLiteTensorsToByteStreamCalculator"
//   input_stream: "TENSORS:tensors"
//   output_stream: "BYTES:bytes"
// }
class TfLiteTensorsToByteStreamCalculator : public CalculatorBase {
 public:
    static absl::Status GetContract(CalculatorContract* cc) {
        RET_CHECK(cc->Inputs().HasTag(kTensorsTag));
        RET_CHECK(cc->Outputs().HasTag(kBytesTag));

        cc->Inputs().Tag(kTensorsTag).Set<std::vector<TfLiteTensor>>();
        cc->Outputs().Tag(kBytesTag).Set<std::string>();

        return absl::OkStatus();
    }

    absl::Status Open(CalculatorContext* cc) override {
        cc->SetOffset(TimestampDiff(0));
        return absl::OkStatus();
    }

    absl::Status Process(CalculatorContext* cc) override {
        RET_CHECK(!cc->Inputs().Tag(kTensorsTag).IsEmpty());

        const auto& input_tensors =
            cc->Inputs().Tag(kTensorsTag).Get<std::vector<TfLiteTensor>>();
        // TODO: Add option to specify which tensor to take from.
        const TfLiteTensor* raw_tensor = &input_tensors[0];
        const float* raw_floats = raw_tensor->data.f;
        int num_values = 1;
        for (int i = 0; i < raw_tensor->dims->size; ++i) {
            RET_CHECK_GT(raw_tensor->dims->data[i], 0);
            num_values *= raw_tensor->dims->data[i];
        }

        // Create a byte stream from the float array
        std::ostringstream oss;
        // Write the size of the vector (4 bytes).
        oss.write(reinterpret_cast<const char*>(&num_values), sizeof(num_values));
        // Write the float data (4 bytes per float).
        oss.write(reinterpret_cast<const char*>(raw_floats), sizeof(float) * num_values);
        std::string bytes = oss.str();

        cc->Outputs()
            .Tag(kBytesTag)
            .AddPacket(MakePacket<std::string>(bytes).At(cc->InputTimestamp()));
        return absl::OkStatus();
    }
    };
    REGISTER_CALCULATOR(TfLiteTensorsToByteStreamCalculator);
}  // namespace mediapipe