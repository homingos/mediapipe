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

#import "MPPTimestampConverter.h"

#include <cstdint>

@implementation MPPTimestampConverter {
  mediapipe::Timestamp _mediapipeTimestamp;
  mediapipe::Timestamp _lastTimestamp;
  mediapipe::TimestampDiff _timestampOffset;
}

- (instancetype)init {
  self = [super init];
  if (self) {
    [self reset];
  }
  return self;
}

- (void)reset {
  _mediapipeTimestamp = mediapipe::Timestamp::Min();
  _lastTimestamp = _mediapipeTimestamp;
  _timestampOffset = 0;
}

- (mediapipe::Timestamp)timestampForMediaTime:(CMTime)mediaTime {
  Float64 sampleSeconds =
      CMTIME_IS_VALID(mediaTime) ? CMTimeGetSeconds(mediaTime) : 0;
  const int64_t sampleUsec =
      sampleSeconds * mediapipe::Timestamp::kTimestampUnitsPerSecond;
  _mediapipeTimestamp = mediapipe::Timestamp(sampleUsec) + _timestampOffset;
  if (_mediapipeTimestamp <= _lastTimestamp) {
    _timestampOffset =
        _timestampOffset + _lastTimestamp + 1 - _mediapipeTimestamp;
    _mediapipeTimestamp = _lastTimestamp + 1;
  }
  _lastTimestamp = _mediapipeTimestamp;
  return _mediapipeTimestamp;
}

@end
