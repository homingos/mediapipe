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

#include "mediapipe/framework/tool/tag_map_helper.h"

#include <string>
#include <vector>

#include "absl/strings/str_cat.h"
#include "mediapipe/framework/port/core_proto_inc.h"
#include "mediapipe/framework/port/logging.h"
#include "mediapipe/framework/port/proto_ns.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/port/status_macros.h"
#include "mediapipe/framework/port/statusor.h"
#include "mediapipe/framework/tool/tag_map.h"

namespace mediapipe {
namespace tool {

// Create using a vector of TAG:<index>:name.
absl::StatusOr<std::shared_ptr<TagMap>> CreateTagMap(
    const std::vector<std::string>& tag_index_names) {
  proto_ns::RepeatedPtrField<ProtoString> fields;
  for (const auto& tag_index_name : tag_index_names) {
    *fields.Add() = tag_index_name;
  }
  return TagMap::Create(fields);
}

// Create using an integer number of entries (for tag "").
absl::StatusOr<std::shared_ptr<TagMap>> CreateTagMap(int num_entries) {
  RET_CHECK_LE(0, num_entries);
  proto_ns::RepeatedPtrField<ProtoString> fields;
  for (int i = 0; i < num_entries; ++i) {
    *fields.Add() = absl::StrCat("name", i);
  }
  return TagMap::Create(fields);
}

// Create using a vector of just tag names.
absl::StatusOr<std::shared_ptr<TagMap>> CreateTagMapFromTags(
    const std::vector<std::string>& tags) {
  proto_ns::RepeatedPtrField<ProtoString> fields;
  for (int i = 0; i < tags.size(); ++i) {
    *fields.Add() = absl::StrCat(tags[i], ":name", i);
  }
  return TagMap::Create(fields);
}

}  // namespace tool
}  // namespace mediapipe
