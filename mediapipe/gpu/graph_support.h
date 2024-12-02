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

// TODO: Update all the reference and delete this forwarding header.
#ifndef MEDIAPIPE_GPU_GRAPH_SUPPORT_H_
#define MEDIAPIPE_GPU_GRAPH_SUPPORT_H_

namespace mediapipe {

static constexpr char kGpuSharedTagName[] = "GPU_SHARED";
static constexpr char kGpuSharedSidePacketName[] = "gpu_shared";
static constexpr char kGpuExecutorName[] = "__gpu";

}  // namespace mediapipe

#endif  // MEDIAPIPE_GPU_GRAPH_SUPPORT_H_
