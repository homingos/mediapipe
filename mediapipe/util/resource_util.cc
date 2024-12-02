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

#include "mediapipe/util/resource_util.h"

#include <iostream>

#include "absl/strings/str_split.h"
#include "mediapipe/framework/deps/file_path.h"
#include "mediapipe/framework/port/file_helpers.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/util/resource_util_custom.h"
#include "mediapipe/util/resource_util_internal.h"

namespace mediapipe {

namespace {
ResourceProviderFn resource_provider_ = nullptr;
}  // namespace

absl::Status GetResourceContents(const std::string& path, std::string* output,
                                 bool read_as_binary) {
  if (resource_provider_) {
    return resource_provider_(path, output);
  }
  return internal::DefaultGetResourceContents(path, output, read_as_binary);
}

bool HasCustomGlobalResourceProvider() { return resource_provider_ != nullptr; }

void SetCustomGlobalResourceProvider(ResourceProviderFn fn) {
  resource_provider_ = std::move(fn);
}

}  // namespace mediapipe
