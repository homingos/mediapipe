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

#ifndef MEDIAPIPE_EXAMPLES_DESKTOP_AUTOFLIP_QUALITY_UTILS_H_
#define MEDIAPIPE_EXAMPLES_DESKTOP_AUTOFLIP_QUALITY_UTILS_H_

#include <vector>

#include "mediapipe/examples/desktop/autoflip/autoflip_messages.pb.h"
#include "mediapipe/examples/desktop/autoflip/quality/cropping.pb.h"
#include "mediapipe/examples/desktop/autoflip/quality/piecewise_linear_function.h"
#include "mediapipe/framework/port/opencv_core_inc.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {
namespace autoflip {

// Packs detected features and timestamp (ms) into a KeyFrameInfo object. Scales
// features back to the original frame size if features have been detected on a
// different frame size.
absl::Status PackKeyFrameInfo(const int64_t frame_timestamp_ms,
                              const DetectionSet& detections,
                              const int original_frame_width,
                              const int original_frame_height,
                              const int feature_frame_width,
                              const int feature_frame_height,
                              KeyFrameInfo* key_frame_info);

// Sorts required and non-required salient regions given a detection set.
absl::Status SortDetections(const DetectionSet& detections,
                            std::vector<SalientRegion>* required_regions,
                            std::vector<SalientRegion>* non_required_regions);

// Sets the target crop size in KeyFrameCropOptions based on frame size and
// target aspect ratio so that the target crop size covers the biggest area
// possible in the frame.
absl::Status SetKeyFrameCropTarget(const int frame_width,
                                   const int frame_height,
                                   const double target_aspect_ratio,
                                   KeyFrameCropOptions* crop_options);

// Aggregates information from KeyFrameInfos and KeyFrameCropResults into
// SceneKeyFrameCropSummary.
absl::Status AggregateKeyFrameResults(
    const KeyFrameCropOptions& key_frame_crop_options,
    const std::vector<KeyFrameCropResult>& key_frame_crop_results,
    const int scene_frame_width, const int scene_frame_height,
    SceneKeyFrameCropSummary* scene_summary);

// Computes the static top and border size across a scene given a vector of
// StaticFeatures over frames.
absl::Status ComputeSceneStaticBordersSize(
    const std::vector<StaticFeatures>& static_features, int* top_border_size,
    int* bottom_border_size);

// Finds the solid background colors in a scene from input StaticFeatures.
// Sets has_solid_background to true if the number of frames with solid
// background color exceeds given threshold, i.e.,
// min_fraction_solid_background_color. Builds the background color
// interpolation functions in Lab space using input timestamps.
absl::Status FindSolidBackgroundColor(
    const std::vector<StaticFeatures>& static_features,
    const std::vector<int64_t>& static_features_timestamps,
    const double min_fraction_solid_background_color,
    bool* has_solid_background,
    PiecewiseLinearFunction* background_color_l_function,
    PiecewiseLinearFunction* background_color_a_function,
    PiecewiseLinearFunction* background_color_b_function);

// Helpers to scale, clamp, and take union of rectangles. These functions do not
// check for pointers not being null or rectangles being valid.

// Scales a rectangle given horizontal and vertical scaling factors.
template <typename T>
void ScaleRect(const T& original_location, const double scale_x,
               const double scale_y, Rect* scaled_location);

// Converts a normalized rectangle to a rectangle given width and height.
void NormalizedRectToRect(const RectF& normalized_location, const int width,
                          const int height, Rect* location);

// Clamps a rectangle to lie within [x0, y0] and [x1, y1]. Returns true if the
// rectangle has any overlapping with the target window.
absl::Status ClampRect(const int x0, const int y0, const int x1, const int y1,
                       Rect* location);

// Convenience function to clamp a rectangle to lie within [0, 0] and
// [width, height].
absl::Status ClampRect(const int width, const int height, Rect* location);

// Enlarges a given rectangle to cover a new rectangle to be added.
void RectUnion(const Rect& rect_to_add, Rect* rect);

// Performs an affine retarget on a list of input images.  Output vector
// cropped_frames must be filled with Mats of the same size as output_size and
// type.
absl::Status AffineRetarget(const cv::Size& output_size,
                            const std::vector<cv::Mat>& frames,
                            const std::vector<cv::Mat>& affine_projection,
                            std::vector<cv::Mat>* cropped_frames);

}  // namespace autoflip
}  // namespace mediapipe

#endif  // MEDIAPIPE_EXAMPLES_DESKTOP_AUTOFLIP_QUALITY_UTILS_H_
