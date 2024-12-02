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

#ifndef JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_GRAPH_TEXTURE_FRAME_JNI_H_
#define JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_GRAPH_TEXTURE_FRAME_JNI_H_

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#define GRAPH_TEXTURE_FRAME_METHOD(METHOD_NAME) \
  Java_com_google_mediapipe_framework_GraphTextureFrame_##METHOD_NAME

// Releases a native mediapipe::GpuBuffer.
JNIEXPORT void JNICALL GRAPH_TEXTURE_FRAME_METHOD(nativeReleaseBuffer)(
    JNIEnv* env, jobject thiz, jlong nativeHandle);

JNIEXPORT jint JNICALL GRAPH_TEXTURE_FRAME_METHOD(nativeGetTextureName)(
    JNIEnv* env, jobject thiz, jlong nativeHandle);

JNIEXPORT void JNICALL GRAPH_TEXTURE_FRAME_METHOD(nativeGpuWait)(
    JNIEnv* env, jobject thiz, jlong nativeHandle);

JNIEXPORT jint JNICALL GRAPH_TEXTURE_FRAME_METHOD(nativeGetWidth)(
    JNIEnv* env, jobject thiz, jlong nativeHandle);

JNIEXPORT jint JNICALL GRAPH_TEXTURE_FRAME_METHOD(nativeGetHeight)(
    JNIEnv* env, jobject thiz, jlong nativeHandle);

JNIEXPORT jlong JNICALL GRAPH_TEXTURE_FRAME_METHOD(
    nativeCreateSyncTokenForCurrentExternalContext)(JNIEnv* env, jobject thiz,
                                                    jlong nativeHandle);

JNIEXPORT void JNICALL GRAPH_TEXTURE_FRAME_METHOD(nativeDidRead)(
    JNIEnv* env, jobject thiz, jlong nativeHandle, jlong consumerSyncToken);

JNIEXPORT jlong JNICALL GRAPH_TEXTURE_FRAME_METHOD(
    nativeGetCurrentExternalContextHandle)(JNIEnv* env, jobject thiz);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_GRAPH_TEXTURE_FRAME_JNI_H_
