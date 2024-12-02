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

#define ABSTRACT_METHOD \
    @throw [NSException exceptionWithName:NSInternalInconsistencyException \
        reason:[NSString stringWithFormat:@"You must override %@ in a subclass", \
                    NSStringFromSelector(_cmd)] \
        userInfo:nil];

@implementation MPPInputSource

- (void)setDelegate:(id<MPPInputSourceDelegate>)delegate
              queue:(dispatch_queue_t)queue {
  _delegate = delegate;
  _delegateQueue = queue;
}

- (void)start {
  ABSTRACT_METHOD
}

- (void)stop {
  ABSTRACT_METHOD
}

@end
