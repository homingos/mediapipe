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

#include "mediapipe/java/com/google/mediapipe/framework/jni/class_registry.h"

#include "absl/strings/str_format.h"

namespace mediapipe {
namespace android {

ClassRegistry::ClassRegistry() {}

ClassRegistry& ClassRegistry::GetInstance() {
  static ClassRegistry* instance_ = new ClassRegistry();
  return *instance_;
}

void ClassRegistry::InstallRenamingMap(
    absl::node_hash_map<std::string, std::string> renaming_map) {
  renaming_map_ = renaming_map;
}

std::string ClassRegistry::GetClassName(std::string cls) {
  auto match = renaming_map_.find(cls);
  if (match != renaming_map_.end()) {
    return match->second;
  }
  return cls;
}

std::string ClassRegistry::GetMethodName(std::string cls, std::string method) {
  std::string key = absl::StrFormat("%s#%s", cls, method);
  auto match = renaming_map_.find(key);
  if (match != renaming_map_.end()) {
    return match->second;
  }
  return method;
}

std::string ClassRegistry::GetFieldName(std::string cls, std::string field) {
  std::string key = absl::StrFormat("%s##%s", cls, field);
  auto match = renaming_map_.find(key);
  if (match != renaming_map_.end()) {
    return match->second;
  }
  return field;
}

}  // namespace android
}  // namespace mediapipe
