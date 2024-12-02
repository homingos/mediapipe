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

#ifndef MEDIAPIPE_GPU_GPU_SERVICE_H_
#define MEDIAPIPE_GPU_GPU_SERVICE_H_

#include "absl/base/attributes.h"
#include "mediapipe/framework/graph_service.h"

#if !MEDIAPIPE_DISABLE_GPU
#include "mediapipe/gpu/gpu_shared_data_internal.h"
#endif  // !MEDIAPIPE_DISABLE_GPU

namespace mediapipe {

#if MEDIAPIPE_DISABLE_GPU
class GpuResources {
  GpuResources() = delete;
};
#endif  // MEDIAPIPE_DISABLE_GPU

ABSL_CONST_INIT extern const GraphService<GpuResources> kGpuService;

}  // namespace mediapipe

#endif  // MEDIAPIPE_GPU_GPU_SERVICE_H_
