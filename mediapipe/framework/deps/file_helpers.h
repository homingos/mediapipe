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

#ifndef MEDIAPIPE_DEPS_FILE_HELPERS_H_
#define MEDIAPIPE_DEPS_FILE_HELPERS_H_

#include "absl/strings/match.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {
namespace file {
absl::Status GetContents(absl::string_view file_name, std::string* output,
                         bool read_as_binary = true);

absl::Status SetContents(absl::string_view file_name,
                         absl::string_view content);

absl::Status AppendStringToFile(absl::string_view file_name,
                                absl::string_view contents);

absl::Status MatchInTopSubdirectories(const std::string& parent_directory,
                                      const std::string& file_name,
                                      std::vector<std::string>* results);

absl::Status MatchFileTypeInDirectory(const std::string& directory,
                                      const std::string& file_suffix,
                                      std::vector<std::string>* results);

absl::Status Exists(absl::string_view file_name);

absl::Status IsDirectory(absl::string_view file_name);

absl::Status RecursivelyCreateDir(absl::string_view path);

}  // namespace file
}  // namespace mediapipe

#endif  // MEDIAPIPE_DEPS_FILE_HELPERS_H_
