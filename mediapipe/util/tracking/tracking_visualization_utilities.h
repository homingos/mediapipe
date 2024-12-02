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

#ifndef MEDIAPIPE_UTIL_TRACKING_TRACKING_VISUALIZATION_UTILITIES_H_
#define MEDIAPIPE_UTIL_TRACKING_TRACKING_VISUALIZATION_UTILITIES_H_

#include "mediapipe/framework/port/opencv_core_inc.h"
#include "mediapipe/util/tracking/box_tracker.pb.h"
#include "mediapipe/util/tracking/flow_packager.pb.h"
#include "mediapipe/util/tracking/tracking.pb.h"

namespace mediapipe {

// Visualizes state to frame. Also overlays statistics if print_stats is set.
void RenderState(const MotionBoxState& box_state, bool print_stats,
                 cv::Mat* frame);

// Visualizes internal tracking state to frame.
void RenderInternalState(const MotionBoxInternalState& internal,
                         cv::Mat* frame);

// Visualizes tracking data (specifically, the motion_vectors) to frame.
// Optional third parameter can be used to disable antialiasing.
void RenderTrackingData(const TrackingData& data, cv::Mat* mat,
                        bool antialiasing = false);

// Visualize TimeBoxProto onto image.
void RenderBox(const TimedBoxProto& box_proto, cv::Mat* mat);

}  // namespace mediapipe

#endif  // MEDIAPIPE_UTIL_TRACKING_TRACKING_VISUALIZATION_UTILITIES_H_
