
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

#import <Foundation/Foundation.h>

#include "mediapipe/framework/port/canonical_errors.h"
#include "mediapipe/framework/profiler/profiler_resource_util.h"

namespace mediapipe {

StatusOr<std::string> GetDefaultTraceLogDirectory() {
  // Get the Documents directory. iOS apps can write files to this directory.
  NSURL* documents_directory_url = [[[NSFileManager defaultManager]
      URLsForDirectory:NSDocumentDirectory
             inDomains:NSUserDomainMask] lastObject];

  // Note: "createDirectoryAtURL:..." method doesn't successfully create
  // the directory, hence this code uses "createDirectoryAtPath:..".
  NSString* ns_documents_directory = [documents_directory_url path];
  NSError* error;
  BOOL success = [[NSFileManager defaultManager]
            createDirectoryAtPath:ns_documents_directory
      withIntermediateDirectories:YES
                       attributes:nil
                            error:&error];
  if (!success) {
    // TODO: Use NSError+util_status to get status from NSError.
    return absl::InternalError([[error localizedDescription] UTF8String]);
  }

  std::string trace_log_directory = [ns_documents_directory UTF8String];
  return trace_log_directory;
}

}  // namespace mediapipe
