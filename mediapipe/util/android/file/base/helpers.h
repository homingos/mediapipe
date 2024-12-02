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

#ifndef MEDIAPIPE_ANDROID_FILE_BASE_HELPERS_H_
#define MEDIAPIPE_ANDROID_FILE_BASE_HELPERS_H_

#include <string>

#include "absl/strings/string_view.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/util/android/file/base/file.h"

namespace mediapipe {
namespace file {

// Read contents of a file to a string.
absl::Status GetContents(absl::string_view file_name, std::string* output,
                         const file::Options& options);

// Read contents of a file to a string with default file options.
absl::Status GetContents(absl::string_view file_name, std::string* output);

// Read contents of a file to a string from an open file descriptor.
absl::Status GetContents(int fd, std::string* output);

// Write string to file.
absl::Status SetContents(absl::string_view file_name, absl::string_view content,
                         const file::Options& options);

// Write string to file with default file options.
absl::Status SetContents(absl::string_view file_name,
                         absl::string_view content);

}  // namespace file
}  // namespace mediapipe

#endif  // MEDIAPIPE_ANDROID_FILE_BASE_HELPERS_H_
