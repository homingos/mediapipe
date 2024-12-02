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

#ifndef MEDIAPIPE_GPU_METAL_SHADER_BASE_H_
#define MEDIAPIPE_GPU_METAL_SHADER_BASE_H_

#include <simd/simd.h>

typedef struct {
  // Vertex position in 2D clip space.
  vector_float2 position;
  // Corresponding texture coordinate.
  vector_float2 texture_coordinate;
} MediaPipeTexturedVertex;

// Common buffer indices used in our Metal shaders.
typedef enum {
  MediaPipeBufferIndexInputVertices = 0,
  MediaPipeBufferIndexRgbWeights = 1,
  MediaPipeBufferIndexPixelSize = 2,
  MediaPipeBufferIndexOutputColor = 3,
} MediaPipeBufferIndex;

// Common texture indices used in our Metal shaders.
typedef enum {
  MediaPipeTextureIndexInputColor = 0,
  MediaPipeTextureIndexOutputColor = 1,
} MediaPipeTextureIndex;

typedef vector_float3 MetalRgbWeightPacketType;

#endif  // MEDIAPIPE_GPU_METAL_SHADER_BASE_H_
