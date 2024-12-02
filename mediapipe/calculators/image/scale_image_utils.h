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
//
// Utilities for scaling operations defined by ScaleImageCalculatorOptions.
#ifndef MEDIAPIPE_IMAGE_SCALE_IMAGE_UTILS_H_
#define MEDIAPIPE_IMAGE_SCALE_IMAGE_UTILS_H_

#include <string>

#include "mediapipe/framework/port/status.h"

namespace mediapipe {
namespace scale_image {

// Given a width and height and min and max aspect ratios, determine the
// target width and height and column and row starts such that the target
// is a centered, cropped portion of the image that falls within the min
// and max aspect ratio.  If either the min or max aspect ratio argument
// is empty or has a 0 in the numerator or denominator then it is ignored.
absl::Status FindCropDimensions(int input_width, int input_height,    //
                                const std::string& min_aspect_ratio,  //
                                const std::string& max_aspect_ratio,  //
                                int* crop_width, int* crop_height,    //
                                int* col_start, int* row_start);

// Given an input width and height, a target width and height or max area,
// whether to preserve the aspect ratio, and whether to round-down to the
// multiple of a given number nearest to the targets, determine the output width
// and height. If target_width or target_height is non-positive, then they will
// be set to the input_width and input_height respectively. If target_area is
// non-positive, then it will be ignored. If scale_to_multiple_of is less than
// 1, it will be treated like 1. The output_width and output_height will be
// reduced as necessary to preserve_aspect_ratio if the option is specified. If
// preserving the aspect ratio is desired, you must set scale_to_multiple_of
// to 2.
absl::Status FindOutputDimensions(int input_width, int input_height,  //
                                  int target_width,
                                  int target_height,           //
                                  int target_max_area,         //
                                  bool preserve_aspect_ratio,  //
                                  int scale_to_multiple_of,    //
                                  int* output_width, int* output_height);

// Backwards compatible helper.
absl::Status FindOutputDimensions(int input_width, int input_height,  //
                                  int target_width,
                                  int target_height,           //
                                  bool preserve_aspect_ratio,  //
                                  int scale_to_multiple_of,    //
                                  int* output_width, int* output_height);

}  // namespace scale_image
}  // namespace mediapipe

#endif  // MEDIAPIPE_IMAGE_SCALE_IMAGE_UTILS_H_
