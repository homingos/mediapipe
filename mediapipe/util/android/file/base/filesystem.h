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

#ifndef MEDIAPIPE_ANDROID_FILE_BASE_FILESYSTEM_H_
#define MEDIAPIPE_ANDROID_FILE_BASE_FILESYSTEM_H_

#include "absl/strings/string_view.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/util/android/file/base/file.h"

namespace mediapipe {
namespace file {

absl::Status RecursivelyCreateDir(absl::string_view path,
                                  const file::Options& options);

absl::Status Exists(absl::string_view path, const file::Options& options);

absl::Status IsDirectory(absl::string_view path, const file::Options& options);

}  // namespace file.
}  // namespace mediapipe

#endif  // MEDIAPIPE_ANDROID_FILE_BASE_FILESYSTEM_H_
