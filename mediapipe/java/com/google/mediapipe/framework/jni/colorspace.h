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

#ifndef JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_COLORSPACE_H_
#define JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_COLORSPACE_H_

#include <cstdint>

namespace mediapipe {
namespace android {
// TODO: switch to more efficient implementation, like halide later.

// Converts an RGBA image to RGB
inline void RgbaToRgb(const uint8_t* rgba_img, int rgba_width_step, int width,
                      int height, uint8_t* rgb_img, int rgb_width_step) {
  for (int y = 0; y < height; ++y) {
    const auto* rgba = rgba_img + y * rgba_width_step;
    auto* rgb = rgb_img + y * rgb_width_step;
    for (int x = 0; x < width; ++x) {
      *rgb = *rgba;
      *(rgb + 1) = *(rgba + 1);
      *(rgb + 2) = *(rgba + 2);
      rgb += 3;
      rgba += 4;
    }
  }
}

// Converts a RGB image to RGBA
inline void RgbToRgba(const uint8_t* rgb_img, int rgb_width_step, int width,
                      int height, uint8_t* rgba_img, int rgba_width_step,
                      uint8_t alpha) {
  for (int y = 0; y < height; ++y) {
    const auto* rgb = rgb_img + y * rgb_width_step;
    auto* rgba = rgba_img + y * rgba_width_step;
    for (int x = 0; x < width; ++x) {
      *rgba = *rgb;
      *(rgba + 1) = *(rgb + 1);
      *(rgba + 2) = *(rgb + 2);
      *(rgba + 3) = alpha;
      rgb += 3;
      rgba += 4;
    }
  }
}

}  // namespace android
}  // namespace mediapipe
#endif  // JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_COLORSPACE_H_
