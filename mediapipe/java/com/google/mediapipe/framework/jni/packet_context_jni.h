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

#ifndef JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_PACKET_CONTEXT_JNI_H_
#define JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_PACKET_CONTEXT_JNI_H_

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#define PACKET_METHOD(METHOD_NAME) \
  Java_com_google_mediapipe_framework_Packet_##METHOD_NAME

// Releases a native mediapipe packet.
JNIEXPORT void JNICALL PACKET_METHOD(nativeReleasePacket)(JNIEnv* env,
                                                          jobject thiz,
                                                          jlong packet);

// Returns the timestamp of the packet.
JNIEXPORT jlong JNICALL PACKET_METHOD(nativeGetTimestamp)(JNIEnv* env,
                                                          jobject thiz,
                                                          jlong packet);

// Returns true if the packet is empty.
JNIEXPORT jboolean JNICALL PACKET_METHOD(nativeIsEmpty)(JNIEnv* env,
                                                        jobject thiz,
                                                        jlong packet);

// Make a copy of a mediapipe packet, basically increase the reference count.
JNIEXPORT jlong JNICALL PACKET_METHOD(nativeCopyPacket)(JNIEnv* env,
                                                        jobject thiz,
                                                        jlong packet);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

// Calls the java method to create an instance of java Packet.
jobject CreateJavaPacket(JNIEnv* env, jclass packet_cls, jlong packet);

#endif  // JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_PACKET_CONTEXT_JNI_H_
