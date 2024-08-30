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
#include "mediapipe/framework/formats/image_frame.h"

namespace mediapipe
{

    constexpr char kVideoTag[] = "VIDEO";
    constexpr char kTFrameTag[] = "TFRAME";

    class VideoSwitcherCalculator : public CalculatorBase
    {
    public:
        static absl::Status GetContract(CalculatorContract *cc);
        absl::Status Open(CalculatorContext *cc) override;
        absl::Status Process(CalculatorContext *cc) override;
    };

    REGISTER_CALCULATOR(VideoSwitcherCalculator);

    absl::Status VideoSwitcherCalculator::GetContract(CalculatorContract *cc)
    {

        cc->Inputs().Tag(kVideoTag).Set<ImageFrame>();
        cc->Inputs().Tag(kTFrameTag).Set<ImageFrame>();
        cc->Outputs().Index(0).Set<ImageFrame>();
        return absl::OkStatus();
    }

    absl::Status VideoSwitcherCalculator::Open(CalculatorContext *cc)
    {
        cc->SetOffset(TimestampDiff(0));
        return absl::OkStatus();
    }

    absl::Status VideoSwitcherCalculator::Process(CalculatorContext *cc)
    {
        if (!cc->Inputs().Tag(kTFrameTag).IsEmpty())
        {
            cc->Outputs()
                .Index(0)
                .Add(
                    &cc->Inputs().Tag(kTFrameTag).Get<ImageFrame>(), cc->InputTimestamp());
        }
        else
        {
            cc->Outputs().Index(0).AddPacket(cc->Inputs().Tag(kVideoTag).Value());
        }
        return absl::OkStatus();
    }

} // namespace mediapipe
