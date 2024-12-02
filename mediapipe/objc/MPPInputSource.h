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

#import <AVFoundation/AVFoundation.h>
#import <CoreAudio/CoreAudioTypes.h>
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class MPPInputSource;

/// A delegate that can receive frames from a source.
@protocol MPPInputSourceDelegate <NSObject>

/// Provides the delegate with a new video frame.
@optional
- (void)processVideoFrame:(CVPixelBufferRef)imageBuffer
               fromSource:(MPPInputSource*)source __deprecated;

/// Provides the delegate with a new video frame.
@optional
- (void)processVideoFrame:(CVPixelBufferRef)imageBuffer
                timestamp:(CMTime)timestamp
               fromSource:(MPPInputSource*)source;

// Provides the delegate with new depth frame data.
@optional
- (void)processDepthData:(AVDepthData*)depthData
               timestamp:(CMTime)timestamp
              fromSource:(MPPInputSource*)source API_AVAILABLE(ios(11.0));

@optional
- (void)videoDidPlayToEnd:(CMTime)timestamp;

// Provides the delegate with the format of the audio track to be played.
@optional
- (void)willStartPlayingAudioWithFormat:(const AudioStreamBasicDescription*)format
                             fromSource:(MPPInputSource*)source;

// Lets the delegate know that there is no audio track despite audio playback
// having been requested (or that audio playback failed for other reasons).
@optional
- (void)noAudioAvailableFromSource:(MPPInputSource*)source;

// Provides the delegate with a new audio packet.
@optional
- (void)processAudioPacket:(const AudioBufferList*)audioPacket
                 numFrames:(CMItemCount)numFrames
                 timestamp:(CMTime)timestamp
                fromSource:(MPPInputSource*)source;

@end

/// Abstract class for a video source.
@interface MPPInputSource : NSObject

/// The delegate that receives the frames.
@property(weak, nonatomic, readonly) id<MPPInputSourceDelegate> delegate;

/// The dispatch queue on which to schedule the delegate callback.
@property(nonatomic, readonly) dispatch_queue_t delegateQueue;

/// Whether the source is currently running.
@property(nonatomic, getter=isRunning, readonly) BOOL running;

/// Sets the delegate and the queue on which its callback should be invoked.
- (void)setDelegate:(id<MPPInputSourceDelegate>)delegate queue:(dispatch_queue_t)queue;

/// CoreVideo pixel format for the video frames. Defaults to
/// kCVPixelFormatType_32BGRA.
@property(nonatomic) OSType pixelFormatType;

/// Starts the source.
- (void)start;

/// Stops the source.
- (void)stop;

@end

NS_ASSUME_NONNULL_END
