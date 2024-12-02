
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

#include "mediapipe/calculators/util/collection_has_min_size_calculator.h"

#include <vector>

#include "mediapipe/framework/formats/classification.pb.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/rect.pb.h"

namespace mediapipe {

typedef CollectionHasMinSizeCalculator<std::vector<mediapipe::NormalizedRect>>
    NormalizedRectVectorHasMinSizeCalculator;
REGISTER_CALCULATOR(NormalizedRectVectorHasMinSizeCalculator);

typedef CollectionHasMinSizeCalculator<
    std::vector<mediapipe::NormalizedLandmarkList>>
    NormalizedLandmarkListVectorHasMinSizeCalculator;
REGISTER_CALCULATOR(NormalizedLandmarkListVectorHasMinSizeCalculator);

typedef CollectionHasMinSizeCalculator<
    std::vector<mediapipe::ClassificationList>>
    ClassificationListVectorHasMinSizeCalculator;
REGISTER_CALCULATOR(ClassificationListVectorHasMinSizeCalculator);

}  // namespace mediapipe
