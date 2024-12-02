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

#import "mediapipe/objc/MPPDisplayLinkWeakTarget.h"

@implementation MPPDisplayLinkWeakTarget {
  __weak id _target;
  SEL _selector;
}

#pragma mark - Init

- (instancetype)initWithTarget:(id)target selector:(SEL)sel {
  self = [super init];
  if (self) {
    _target = target;
    _selector = sel;
  }
  return self;
}

#pragma mark - Public

- (void)displayLinkCallback:(CADisplayLink *)sender {
  __strong id target = _target;
  if (target == nil) {
    return;
  }
  void (*display)(id, SEL, CADisplayLink *) = (void *)[target methodForSelector:_selector];
  display(target, _selector, sender);
}

@end
