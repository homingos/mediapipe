// Copyright 2024 Flamingos Technologies Inc. All Rights Reserved.
//
// PROPRIETARY AND CONFIDENTIAL
//
// This software and its documentation are the confidential and proprietary
// information of Flamingos Technologies Inc. ("Confidential Information").
// You shall not disclose such Confidential Information and shall use it
// only in accordance with the terms of the license agreement you entered
// into with Flamingos Technologies Inc. Unauthorized copying, redistribution,
// or use of this software in source or binary forms is strictly prohibited.

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
