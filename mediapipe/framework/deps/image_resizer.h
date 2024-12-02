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

#ifndef MEDIAPIPE_DEPS_IMAGE_RESIZER_H_
#define MEDIAPIPE_DEPS_IMAGE_RESIZER_H_

#include "mediapipe/framework/port/opencv_imgproc_inc.h"

namespace mediapipe {

class ImageResizer {
 public:
  ImageResizer(double sharpen_coeff) {}

  bool Resize(const cv::Mat& input_mat, cv::Mat* output_mat) {
    cv::resize(input_mat, *output_mat, output_mat->size(), 0, 0,
               cv::INTER_AREA);
    return true;
  }
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_DEPS_IMAGE_RESIZER_H_
