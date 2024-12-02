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

#ifndef MEDIAPIPE_OBJC_CGIMAGEREFUTILS_H_
#define MEDIAPIPE_OBJC_CGIMAGEREFUTILS_H_

#import <CoreVideo/CoreVideo.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

extern NSString *const kCGImageRefUtilsErrorDomain;

// TODO: Get rid of this library or make it a wrapper around util.h
// versions so that it can be used in pure Objective-C code.

/// Creates a CGImage with a copy of the contents of the CVPixelBuffer. Returns nil on error, if
/// the |error| argument is not nil, *error is set to an NSError describing the failure. Caller
/// is responsible for releasing the CGImage by calling CGImageRelease().
CGImageRef CreateCGImageFromCVPixelBuffer(CVPixelBufferRef imageBuffer, NSError **error);

/// Creates a CVPixelBuffer with a copy of the contents of the CGImage. Returns nil on error, if
/// the |error| argument is not nil, *error is set to an NSError describing the failure. Caller
/// is responsible for releasing the CVPixelBuffer by calling CVPixelBufferRelease.
CVPixelBufferRef CreateCVPixelBufferFromCGImage(CGImageRef image, NSError **error);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // MEDIAPIPE_OBJC_CGIMAGEREFUTILS_H_
