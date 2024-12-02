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
#import <Foundation/Foundation.h>

/// A generic target/callback holder. Useful for indirectly using DisplayLink and allowing the
/// complete deletion of displaylink reference holders.
@interface MPPDisplayLinkWeakTarget : NSObject

- (instancetype)initWithTarget:(id)target selector:(SEL)sel;

- (void)displayLinkCallback:(CADisplayLink *)sender;

@end
