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

#include "mediapipe/calculators/util/filter_collection_calculator.h"

#include <cstdint>
#include <vector>

#include "mediapipe/framework/formats/classification.pb.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/rect.pb.h"

namespace mediapipe {

typedef FilterCollectionCalculator<std::vector<uint64_t>>
    FilterUInt64CollectionCalculator;
REGISTER_CALCULATOR(FilterUInt64CollectionCalculator);

typedef FilterCollectionCalculator<std::vector<::mediapipe::NormalizedRect>>
    FilterNormalizedRectCollectionCalculator;
REGISTER_CALCULATOR(FilterNormalizedRectCollectionCalculator);

typedef FilterCollectionCalculator<std::vector<::mediapipe::LandmarkList>>
    FilterLandmarkListCollectionCalculator;
REGISTER_CALCULATOR(FilterLandmarkListCollectionCalculator);

typedef FilterCollectionCalculator<
    std::vector<::mediapipe::NormalizedLandmarkList>>
    FilterNormalizedLandmarkListCollectionCalculator;
REGISTER_CALCULATOR(FilterNormalizedLandmarkListCollectionCalculator);

typedef FilterCollectionCalculator<std::vector<::mediapipe::ClassificationList>>
    FilterClassificationListCollectionCalculator;
REGISTER_CALCULATOR(FilterClassificationListCollectionCalculator);

}  // namespace mediapipe
