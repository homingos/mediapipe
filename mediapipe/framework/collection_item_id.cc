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

#include "mediapipe/framework/collection_item_id.h"

namespace mediapipe {

std::ostream& operator<<(std::ostream& os, CollectionItemId arg) {
  return os << arg.value();
}

CollectionItemId operator+(int lhs, CollectionItemId rhs) { return rhs + lhs; }
CollectionItemId operator-(int lhs, CollectionItemId rhs) { return -rhs + lhs; }
CollectionItemId operator*(int lhs, CollectionItemId rhs) { return rhs * lhs; }

}  // namespace mediapipe
