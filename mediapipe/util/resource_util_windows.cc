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
#include "mediapipe/framework/port/singleton.h"
#include "mediapipe/framework/port/statusor.h"
#include "tools/cpp/runfiles/runfiles.h"

ABSL_FLAG(
    std::string, resource_root_dir, "",
    "The absolute path to the resource directory."
    "If specified, resource_root_dir will be prepended to the original path.");

namespace mediapipe {

using mediapipe::file::GetContents;
using mediapipe::file::JoinPath;

namespace internal {
namespace {

class RunfilesHolder {
 public:
  // TODO: We should ideally use `CreateForTests` when this is
  // accessed from unit tests.
  RunfilesHolder()
      : runfiles_(
            ::bazel::tools::cpp::runfiles::Runfiles::Create("", nullptr)) {}

  std::string Rlocation(const std::string& path) {
    if (!runfiles_) {
      // Return the original path when Runfiles is not available (e.g. for
      // Python)
      return JoinPath(absl::GetFlag(FLAGS_resource_root_dir), path);
    }
    return runfiles_->Rlocation(path);
  }

 private:
  std::unique_ptr<::bazel::tools::cpp::runfiles::Runfiles> runfiles_;
};

std::string PathToResourceAsFileInternal(const std::string& path) {
  return Singleton<RunfilesHolder>::get()->Rlocation(path);
}

}  // namespace

absl::Status DefaultGetResourceContents(const std::string& path,
                                        std::string* output,
                                        bool read_as_binary) {
  std::string resource_path = PathToResourceAsFileInternal(path);
  return GetContents(path, output, read_as_binary);
}

}  // namespace internal

absl::StatusOr<std::string> PathToResourceAsFile(const std::string& path) {
  std::string qualified_path = path;
  if (absl::StartsWith(qualified_path, "./")) {
    qualified_path = "mediapipe" + qualified_path.substr(1);
  } else if (path[0] != '/') {
    qualified_path = "mediapipe/" + qualified_path;
  }

  // Try to load the file from bazel-bin. If it does not exist, fall back to the
  // resource folder.
  auto bazel_path = internal::PathToResourceAsFileInternal(qualified_path);
  if (file::Exists(bazel_path).ok()) {
    return bazel_path;
  }
  return JoinPath(absl::GetFlag(FLAGS_resource_root_dir), path);
}

}  // namespace mediapipe
