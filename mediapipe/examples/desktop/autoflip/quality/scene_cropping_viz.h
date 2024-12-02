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

#include "mediapipe/examples/desktop/autoflip/quality/focus_point.pb.h"
#ifndef MEDIAPIPE_EXAMPLES_DESKTOP_AUTOFLIP_QUALITY_SCENE_CROPPING_VIZ_H_
#define MEDIAPIPE_EXAMPLES_DESKTOP_AUTOFLIP_QUALITY_SCENE_CROPPING_VIZ_H_

#include <vector>

#include "mediapipe/examples/desktop/autoflip/quality/cropping.pb.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/image_format.pb.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/port/opencv_core_inc.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {
namespace autoflip {

// Draws the detections and crop regions on the scene frame. To make
// visualization smoother, applies piecewise-constant interpolation on non-key
// frames. This helps visualize the inputs to and outputs from the
// FrameCropRegionComputer. Uses thick green for computed crop regions. Uses
// different colors for different focus signals, faces are green, motion is
// magenta, logos are red, ocrs are yellow (foreground) and light yellow
// (background), brain objects are cyan, ica objects are orange, and the rest
// are white.
absl::Status DrawDetectionsAndCropRegions(
    const std::vector<cv::Mat>& scene_frames,
    const std::vector<bool>& is_key_frames,
    const std::vector<KeyFrameInfo>& key_frame_infos,
    const std::vector<KeyFrameCropResult>& key_frame_crop_results,
    const mediapipe::ImageFormat::Format image_format,
    std::vector<std::unique_ptr<ImageFrame>>* viz_frames);

// Draws the focus point from the given FocusPointFrame and the crop window
// centered around it on the scene frame in red. This helps visualize the input
// to the retargeter.
absl::Status DrawFocusPointAndCropWindow(
    const std::vector<cv::Mat>& scene_frames,
    const std::vector<FocusPointFrame>& focus_point_frames,
    const float overlay_opacity, const int crop_window_width,
    const int crop_window_height,
    const mediapipe::ImageFormat::Format image_format,
    std::vector<std::unique_ptr<ImageFrame>>* viz_frames);

// Draws the final smoothed path of the camera retargeter by darkening the
// removed areas.
absl::Status DrawDetectionAndFramingWindow(
    const std::vector<cv::Mat>& org_scene_frames,
    const std::vector<cv::Rect>& crop_from_locations,
    const ImageFormat::Format image_format, const float overlay_opacity,
    std::vector<std::unique_ptr<ImageFrame>>* viz_frames);

}  // namespace autoflip
}  // namespace mediapipe

#endif  // MEDIAPIPE_EXAMPLES_DESKTOP_AUTOFLIP_QUALITY_SCENE_CROPPING_VIZ_H_
