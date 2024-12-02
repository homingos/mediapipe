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

#ifndef MEDIAPIPE_UTIL_RESOURCE_UTIL_H_
#define MEDIAPIPE_UTIL_RESOURCE_UTIL_H_

#include <string>

#include "mediapipe/framework/port/statusor.h"

namespace mediapipe {

// Given a path to a resource, this function attempts to provide an absolute
// path with which it can be accessed as a file.
// - If the input path is an absolute path, it is returned as-is.
// - If the input path is relative, it is searched in a platform-specific
//   location:
//   - On Android, we look for an asset with the given relative path; if
//     it exists, it is copied to the file system (using the AssetCache),
//     and a path to that file is returned.
//   - On iOS, we look for a resource with the given relative path in the
//     application bundle.
//
// Note: The exact search algorithm is subject to change.
// Note: This function should be used by code that needs a resource to be
// accessible as a normal file, usually to call an existing API that only
// accepts file paths. Code that can access data as a stream or as a buffer
// should read from an asset directly on Android; an API for this will be
// provided later. TODO.
absl::StatusOr<std::string> PathToResourceAsFile(const std::string& path);

// Reads the entire contents of a resource. The search path is as in
// PathToResourceAsFile.
absl::Status GetResourceContents(const std::string& path, std::string* output,
                                 bool read_as_binary = true);

}  // namespace mediapipe

#endif  // MEDIAPIPE_UTIL_RESOURCE_UTIL_H_
