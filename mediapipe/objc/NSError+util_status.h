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

#import "Foundation/Foundation.h"

#include "mediapipe/framework/port/status.h"

/// Error domain for absl::Status errors.
extern NSString *const kGUSGoogleUtilStatusErrorDomain;

/// Key for the absl::Status wrapper in an NSError's user info dictionary.
extern NSString *const kGUSGoogleUtilStatusErrorKey;

/// This just wraps absl::Status into an Objective-C object.
@interface GUSUtilStatusWrapper : NSObject

@property(nonatomic) absl::Status status;

+ (instancetype)wrapStatus:(const absl::Status &)status;

@end

/// This category adds methods for generating NSError objects from absl::Status
/// objects, and vice versa.
@interface NSError (GUSGoogleUtilStatus)

/// Generates an NSError representing a absl::Status. Note that NSError always
/// represents an error, so this should not be called with absl::Status::OK.
+ (NSError *)gus_errorWithStatus:(const absl::Status &)status;

/// Returns a absl::Status object representing an NSError. If the NSError was
/// generated from a absl::Status, the absl::Status returned is identical to
/// the original. Otherwise, this returns a status with code ::util::error::UNKNOWN
/// and a message extracted from the NSError.
@property(nonatomic, readonly) absl::Status gus_status;  // NOLINT(identifier-naming)

@end
