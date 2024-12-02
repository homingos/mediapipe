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

#ifndef MEDIAPIPE_PORT_OPENCV_IMGPROC_INC_H_
#define MEDIAPIPE_PORT_OPENCV_IMGPROC_INC_H_

#include <opencv2/core/version.hpp>

#include "mediapipe/framework/port/opencv_core_inc.h"

#ifdef CV_VERSION_EPOCH // for OpenCV 2.x
#include <opencv2/imgproc/imgproc.hpp>
#else
#include <opencv2/imgproc.hpp>
#if CV_VERSION_MAJOR == 4
#include <opencv2/imgproc/types_c.h>
#endif
#endif

#endif // MEDIAPIPE_PORT_OPENCV_IMGPROC_INC_H_
