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

#include "mediapipe/java/com/google/mediapipe/framework/jni/graph_service_jni.h"

#include "mediapipe/java/com/google/mediapipe/framework/jni/graph.h"

namespace mediapipe {
namespace android {

void GraphServiceHelper::SetServicePacket(jlong context_handle,
                                          const GraphServiceBase& service,
                                          Packet packet) {
  mediapipe::android::Graph* mediapipe_graph =
      reinterpret_cast<mediapipe::android::Graph*>(context_handle);
  mediapipe_graph->SetServicePacket(service, packet);
}

}  // namespace android
}  // namespace mediapipe
