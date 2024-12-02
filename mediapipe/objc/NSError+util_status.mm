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

#import "mediapipe/objc/NSError+util_status.h"

@implementation GUSUtilStatusWrapper

+ (instancetype)wrapStatus:(const absl::Status &)status {
  return [[self alloc] initWithStatus:status];
}

- (instancetype)initWithStatus:(const absl::Status &)status {
  self = [super init];
  if (self) {
    _status = status;
  }
  return self;
}

- (NSString *)description {
  return [NSString stringWithFormat:@"<%@: %p; status = %s>",
          [self class], self, _status.message().data()];
}

@end

@implementation NSError (GUSGoogleUtilStatus)

NSString *const kGUSGoogleUtilStatusErrorDomain = @"GoogleUtilStatusErrorDomain";
NSString *const kGUSGoogleUtilStatusErrorKey = @"GUSGoogleUtilStatusErrorKey";

+ (NSError *)gus_errorWithStatus:(const absl::Status &)status {
  NSDictionary *userInfo = @{
    NSLocalizedDescriptionKey : @(status.message().data()),
    kGUSGoogleUtilStatusErrorKey : [GUSUtilStatusWrapper wrapStatus:status],
  };
  NSError *error = [NSError errorWithDomain:kGUSGoogleUtilStatusErrorDomain
                                       code:static_cast<NSInteger>(status.code())
                                   userInfo:userInfo];
  return error;
}

- (absl::Status)gus_status {
  NSString *domain = self.domain;
  if ([domain isEqual:kGUSGoogleUtilStatusErrorDomain]) {
    GUSUtilStatusWrapper *wrapper = self.userInfo[kGUSGoogleUtilStatusErrorKey];
    if (wrapper) return wrapper.status;
#if 0
  // Unfortunately, util/task/posixerrorspace.h is not in portable status yet.
  // TODO: fix that.
  } else if ([domain isEqual:NSPOSIXErrorDomain]) {
    return ::util::PosixErrorToStatus(self.code, self.localizedDescription.UTF8String);
#endif
  }
  return absl::Status(absl::StatusCode::kUnknown, self.localizedDescription.UTF8String);
}

@end
