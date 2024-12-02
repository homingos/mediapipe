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

#import "MPPInputSource.h"

/// A source that outputs frames from a video, played in real time.
/// Not meant for batch processing of video.
@interface MPPPlayerInputSource : MPPInputSource

/// Initializes the video source with optional audio processing.
///
/// @param video The video asset to play.
/// @param audioProcessingEnabled If set, indicates that the (first) audio track
///        should be processed if it exists, and the corresponding methods for
///        audio will be invoked on the @c delegate.
- (instancetype)initWithAVAsset:(AVAsset*)video audioProcessingEnabled:(BOOL)audioProcessingEnabled;

/// Initializes the video source to process @c video with audio processing disabled.
- (instancetype)initWithAVAsset:(AVAsset*)video;

/// Skip into video @c time from beginning (time 0), within error of +/- tolerance to closest time.
- (void)seekToTime:(CMTime)time tolerance:(CMTime)tolerance;

/// Set time into video at which to end playback.
- (void)setPlaybackEndTime:(CMTime)time;

/// Returns the current video's timestamp.
- (CMTime)currentPlayerTime;

@end
