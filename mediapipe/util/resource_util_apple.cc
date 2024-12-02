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

#include <fstream>
#include <sstream>

#include "absl/log/absl_log.h"
#include "absl/strings/match.h"
#include "mediapipe/framework/port/file_helpers.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/port/statusor.h"
#include "mediapipe/util/resource_util.h"

namespace mediapipe {

namespace {
absl::StatusOr<std::string> PathToResourceAsFileInternal(
    const std::string& path) {
  NSString* ns_path = [NSString stringWithUTF8String:path.c_str()];
  Class mediapipeGraphClass = NSClassFromString(@"MPPGraph");
  NSString* resource_dir =
      [[NSBundle bundleForClass:mediapipeGraphClass] resourcePath];
  NSString* resolved_ns_path =
      [resource_dir stringByAppendingPathComponent:ns_path];
  std::string resolved_path = [resolved_ns_path UTF8String];
  RET_CHECK([[NSFileManager defaultManager] fileExistsAtPath:resolved_ns_path])
      << "cannot find file: " << resolved_path;
  return resolved_path;
}
}  // namespace

namespace internal {
absl::Status DefaultGetResourceContents(const std::string& path,
                                        std::string* output,
                                        bool read_as_binary) {
  if (!read_as_binary) {
    ABSL_LOG(WARNING)
        << "Setting \"read_as_binary\" to false is a no-op on ios.";
  }
  MP_ASSIGN_OR_RETURN(std::string full_path, PathToResourceAsFile(path));
  return file::GetContents(full_path, output, read_as_binary);
}
}  // namespace internal

absl::StatusOr<std::string> PathToResourceAsFile(const std::string& path) {
  // Return full path.
  if (absl::StartsWith(path, "/")) {
    return path;
  }

  // Try to load a relative path or a base filename as is.
  {
    auto status_or_path = PathToResourceAsFileInternal(path);
    if (status_or_path.ok()) {
      ABSL_LOG(INFO) << "Successfully loaded: " << path;
      return status_or_path;
    }
  }

  // If that fails, assume it was a relative path, and try just the base name.
  {
    const size_t last_slash_idx = path.find_last_of("\\/");
    RET_CHECK(last_slash_idx != std::string::npos)
        << path << " doesn't have a slash in it";  // Make sure it's a path.
    auto base_name = path.substr(last_slash_idx + 1);
    auto status_or_path = PathToResourceAsFileInternal(base_name);
    if (status_or_path.ok()) {
      ABSL_LOG(INFO) << "Successfully loaded: " << base_name;
      return status_or_path;
    }
  }

  // Try the test environment.
  {
    absl::string_view workspace = "mediapipe";
    const char* test_srcdir = std::getenv("TEST_SRCDIR");
    auto test_path =
        file::JoinPath(test_srcdir ? test_srcdir : "", workspace, path);
    if ([[NSFileManager defaultManager]
            fileExistsAtPath:[NSString
                                 stringWithUTF8String:test_path.c_str()]]) {
      ABSL_LOG(INFO) << "Successfully loaded: " << test_path;
      return test_path;
    }
  }

  return path;
}

}  // namespace mediapipe
