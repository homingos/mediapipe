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

#include "mediapipe/framework/deps/registration.h"

#include "absl/container/flat_hash_set.h"

namespace mediapipe {

namespace {

// List of namespaces that can register calculators inside the namespace
// and still refer to them using an unqualified name.  This allowlist
// is meant to facilitate migration from unqualified to fully qualified
// calculator names.
constexpr char const* kTopNamespaces[] = {
    "mediapipe",
};

template <size_t SIZE, class T>
inline size_t array_size(T (&arr)[SIZE]) {
  return SIZE;
}

}  // namespace

/*static*/
const absl::flat_hash_set<std::string>& NamespaceAllowlist::TopNamespaces() {
  static absl::flat_hash_set<std::string>* result =
      new absl::flat_hash_set<std::string>(
          kTopNamespaces, kTopNamespaces + array_size(kTopNamespaces));
  return *result;
}

}  // namespace mediapipe
