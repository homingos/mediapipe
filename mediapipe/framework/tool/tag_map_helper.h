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

#ifndef MEDIAPIPE_FRAMEWORK_TOOL_TAG_MAP_HELPER_H_
#define MEDIAPIPE_FRAMEWORK_TOOL_TAG_MAP_HELPER_H_

#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/port/statusor.h"
#include "mediapipe/framework/tool/tag_map.h"

namespace mediapipe {
namespace tool {

// Create a TagMap using a vector of TAG:<index>:name.
absl::StatusOr<std::shared_ptr<TagMap>> CreateTagMap(
    const std::vector<std::string>& tag_index_names);

// Create a TagMap using an integer number of entries (for tag "").
absl::StatusOr<std::shared_ptr<TagMap>> CreateTagMap(int num_entries);

// Create a TagMap using a vector of just tag names.
absl::StatusOr<std::shared_ptr<TagMap>> CreateTagMapFromTags(
    const std::vector<std::string>& tags);

}  // namespace tool
}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_TOOL_TAG_MAP_HELPER_H_
