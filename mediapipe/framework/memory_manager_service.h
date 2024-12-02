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
#ifndef MEDIAPIPE_FRAMEWORK_MEMORY_MANAGER_SERVICE_H_
#define MEDIAPIPE_FRAMEWORK_MEMORY_MANAGER_SERVICE_H_

#include "mediapipe/framework/graph_service.h"
#include "mediapipe/framework/memory_manager.h"

namespace mediapipe {

// Graph service to request pooled buffer objects.
inline constexpr GraphService<MemoryManager> kMemoryManagerService(
    "MemoryManagerService", GraphServiceBase::kDisallowDefaultInitialization);

}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_MEMORY_MANAGER_SERVICE_H_
