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

#ifndef MEDIAPIPE_PORT_OPENCV_CORE_INC_H_
#define MEDIAPIPE_PORT_OPENCV_CORE_INC_H_

#include <opencv2/core/version.hpp>
#ifdef CV_VERSION_EPOCH // for OpenCV 2.x
#include <opencv2/core/core.hpp>
#else
#if CV_VERSION_MAJOR == 3
#include <opencv2/cvconfig.h>
#endif

#include <opencv2/core.hpp>
#endif

#endif // MEDIAPIPE_PORT_OPENCV_CORE_INC_H_
