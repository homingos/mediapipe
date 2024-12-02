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

#include "mediapipe/java/com/google/mediapipe/framework/jni/packet_context_jni.h"

#include "absl/strings/str_format.h"
#include "mediapipe/java/com/google/mediapipe/framework/jni/class_registry.h"
#include "mediapipe/java/com/google/mediapipe/framework/jni/graph.h"

// Releases a native mediapipe packet.
JNIEXPORT void JNICALL PACKET_METHOD(nativeReleasePacket)(JNIEnv* env,
                                                          jobject thiz,
                                                          jlong packet) {
  // Removes the packet from the mediapipe context.
  mediapipe::android::Graph::RemovePacket(packet);
}

JNIEXPORT jlong JNICALL PACKET_METHOD(nativeGetTimestamp)(JNIEnv* env,
                                                          jobject thiz,
                                                          jlong packet) {
  return mediapipe::android::Graph::GetPacketFromHandle(packet)
      .Timestamp()
      .Value();
}

JNIEXPORT jboolean JNICALL PACKET_METHOD(nativeIsEmpty)(JNIEnv* env,
                                                        jobject thiz,
                                                        jlong packet) {
  return mediapipe::android::Graph::GetPacketFromHandle(packet).IsEmpty();
}

JNIEXPORT jlong JNICALL PACKET_METHOD(nativeCopyPacket)(JNIEnv* env,
                                                        jobject thiz,
                                                        jlong packet) {
  auto mediapipe_graph =
      mediapipe::android::Graph::GetContextFromHandle(packet);
  mediapipe::Packet mediapipe_packet =
      mediapipe::android::Graph::GetPacketFromHandle(packet);
  return mediapipe_graph->WrapPacketIntoContext(mediapipe_packet);
}

jobject CreateJavaPacket(JNIEnv* env, jclass packet_cls, jlong packet) {
  auto& class_registry = mediapipe::android::ClassRegistry::GetInstance();

  std::string packet_class_name = class_registry.GetClassName(
      mediapipe::android::ClassRegistry::kPacketClassName);
  std::string create_method_name = class_registry.GetMethodName(
      mediapipe::android::ClassRegistry::kPacketClassName, "create");

  std::string signature = absl::StrFormat("(J)L%s;", packet_class_name);
  jmethodID createMethod = env->GetStaticMethodID(
      packet_cls, create_method_name.c_str(), signature.c_str());
  return env->CallStaticObjectMethod(packet_cls, createMethod, packet);
}
