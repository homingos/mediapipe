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

#import <CoreMedia/CoreMedia.h>
#import <Foundation/Foundation.h>

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/objc/util.h"

/// Helps convert a CMTime to a MediaPipe timestamp.
@interface MPPTimestampConverter : NSObject

/// The last timestamp returned by timestampForMediaTime:.
@property(nonatomic, readonly) mediapipe::Timestamp lastTimestamp;

/// Initializer.
- (instancetype)init NS_DESIGNATED_INITIALIZER;

/// Resets the object. After this method is called, we can return timestamps
/// that are lower than previously returned timestamps.
- (void)reset;

/// Converts a CMTime to a MediaPipe timestamp. This ensures that MediaPipe
/// timestamps
/// are always increasing: if the provided CMTime has gone backwards (e.g. if
/// it's from a
/// looping video), we shift all timestamps from that point on to keep the
/// output increasing.
/// This state is erased when reset is called.
- (mediapipe::Timestamp)timestampForMediaTime:(CMTime)mediaTime;

@end
