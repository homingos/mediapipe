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

#include <fstream>

#include "absl/flags/flag.h"
#include "mediapipe/framework/deps/file_path.h"
#include "mediapipe/framework/port/file_helpers.h"
#include "mediapipe/framework/port/statusor.h"

ABSL_FLAG(
    std::string, resource_root_dir, "",
    "The absolute path to the resource directory."
    "If specified, resource_root_dir will be prepended to the original path.");

namespace mediapipe {

using mediapipe::file::GetContents;
using mediapipe::file::JoinPath;

namespace internal {

absl::Status DefaultGetResourceContents(const std::string& path,
                                        std::string* output,
                                        bool read_as_binary) {
  return GetContents(path, output, read_as_binary);
}
}  // namespace internal

absl::StatusOr<std::string> PathToResourceAsFile(const std::string& path) {
  if (absl::StartsWith(path, "/")) {
    return path;
  }

  // Try to load the file from bazel-bin. If it does not exist, fall back to the
  // resource folder.
  auto bazel_path = JoinPath("bazel-bin", path);
  if (file::Exists(bazel_path).ok()) {
    return bazel_path;
  }
  return JoinPath(absl::GetFlag(FLAGS_resource_root_dir), path);
}

}  // namespace mediapipe
