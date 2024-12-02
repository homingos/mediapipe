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
// Small helper function for RegionFlow Visualization.
#ifndef MEDIAPIPE_UTIL_TRACKING_REGION_FLOW_VISUALIZATION_H_
#define MEDIAPIPE_UTIL_TRACKING_REGION_FLOW_VISUALIZATION_H_

#include <algorithm>
#include <cmath>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "mediapipe/framework/port/logging.h"
#include "mediapipe/framework/port/opencv_core_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/vector.h"
#include "mediapipe/util/tracking/motion_models.h"
#include "mediapipe/util/tracking/region_flow.pb.h"  // NOLINT

namespace mediapipe {

typedef RegionFlowFrame::RegionFlow RegionFlow;
typedef std::vector<RegionFlowFeature*> RegionFlowFeatureView;

// Visualizes each tracked feature by a line connecting original and tracked
// points. Uses red for foreground features and blue for background.
// Output is expected to be a VALID 3 channel 8 bit image, clipping of lines
// is performed if points are out of bound. Image should be allocated with the
// same size that was used to compute flow_frame.
void VisualizeRegionFlow(const RegionFlowFrame& region_flow_frame,
                         cv::Mat* output);

// Visualizes tracked features by lines connecting original and tracked
// points. Image is expected to be a VALID 3 channel 8 bit image,
// clipping of lines is performed if points are out of bound.
// For color, use for example cv::Scalar(0, 255, 0). If outlier_color != color,
// each feature is colored by linearly blending the two colors w.r.t. its
// irls_weight (if irls_visualization is set), otherwise feature descriptor
// patch variance is used for visualization (color for high variance,
// outlier_color for low variance).
// Parameters scale_x and scale_y allow for scaling features and matches
// uniformly before plotting to output.
// TODO: Retire irls_visualization.
void VisualizeRegionFlowFeatures(const RegionFlowFeatureList& feature_list,
                                 const cv::Scalar& color,
                                 const cv::Scalar& outlier,
                                 bool irls_visualization,
                                 float scale_x,  // use 1.0 for no scaling
                                 float scale_y,  // ditto.
                                 cv::Mat* output);

// Similar to above, visualizes tracks as lines over time.
// Tracks with length smaller than min_track_length are not visualized. Track
// visualization is limited to max_points_per_track.
class LongFeatureStream;

void VisualizeLongFeatureStream(const LongFeatureStream& stream,
                                const cv::Scalar& color,
                                const cv::Scalar& outlier, int min_track_length,
                                int max_points_per_track, float scale_x,
                                float scale_y, cv::Mat* output);

}  // namespace mediapipe
#endif  // MEDIAPIPE_UTIL_TRACKING_REGION_FLOW_VISUALIZATION_H_
