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

#import "CGImageRefUtils.h"

#import <Foundation/Foundation.h>
#import "mediapipe/objc/CFHolder.h"
#import "mediapipe/objc/NSError+util_status.h"
#import "mediapipe/objc/util.h"

#include "mediapipe/framework/port/status.h"

CGImageRef CreateCGImageFromCVPixelBuffer(CVPixelBufferRef imageBuffer, NSError **error) {
  CFHolder<CGImageRef> cg_image_holder;
  absl::Status status = CreateCGImageFromCVPixelBuffer(imageBuffer, &cg_image_holder);
  if (!status.ok()) {
    *error = [NSError gus_errorWithStatus:status];
    return nil;
  }
  CGImageRef cg_image = *cg_image_holder;
  CGImageRetain(cg_image);
  return cg_image;
}

CVPixelBufferRef CreateCVPixelBufferFromCGImage(CGImageRef image, NSError **error) {
  CFHolder<CVPixelBufferRef> pixel_buffer_holder;
  absl::Status status = CreateCVPixelBufferFromCGImage(image, &pixel_buffer_holder);
  if (!status.ok()) {
    *error = [NSError gus_errorWithStatus:status];
    return nil;
  }
  CVPixelBufferRef pixel_buffer = *pixel_buffer_holder;
  CVPixelBufferRetain(pixel_buffer);
  return pixel_buffer;
}
