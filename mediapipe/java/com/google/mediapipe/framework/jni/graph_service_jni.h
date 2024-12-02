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

#ifndef JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_GRAPH_SERVICE_JNI_H_
#define JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_GRAPH_SERVICE_JNI_H_

#include <jni.h>

#include "mediapipe/framework/graph_service.h"
#include "mediapipe/framework/packet.h"

namespace mediapipe {
namespace android {

// Support class for handling graph services in JNI.
// It keeps the context argument opaque and avoids exposing the entire
// Graph to service JNI implementations.
class GraphServiceHelper {
 public:
  // Call this static method to provide a native service object in response to
  // a call to GraphService#installServiceObject in Java.
  // The context_handle parameter should be the same as passed to
  // installServiceObject.
  template <typename T>
  static void SetServiceObject(jlong context_handle,
                               const GraphService<T>& service,
                               std::shared_ptr<T> object) {
    SetServicePacket(context_handle, service,
                     MakePacket<std::shared_ptr<T>>(std::move(object)));
  }

 private:
  static void SetServicePacket(jlong context_handle,
                               const GraphServiceBase& service, Packet packet);
};

}  // namespace android
}  // namespace mediapipe

#endif  // JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_GRAPH_SERVICE_JNI_H_
