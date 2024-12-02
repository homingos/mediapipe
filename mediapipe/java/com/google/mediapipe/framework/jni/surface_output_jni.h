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

#ifndef JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_SURFACE_OUTPUT_JNI_H_
#define JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_SURFACE_OUTPUT_JNI_H_

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#define MEDIAPIPE_SURFACE_OUTPUT_METHOD(METHOD_NAME) \
  Java_com_google_mediapipe_framework_SurfaceOutput_##METHOD_NAME

JNIEXPORT void JNICALL MEDIAPIPE_SURFACE_OUTPUT_METHOD(nativeSetFlipY)(
    JNIEnv* env, jobject thiz, jlong packet, jboolean flip);

JNIEXPORT void JNICALL MEDIAPIPE_SURFACE_OUTPUT_METHOD(
    nativeSetUpdatePresentationTime)(JNIEnv* env, jobject thiz, jlong packet,
                                     jboolean update_presentation_time);

#ifdef __ANDROID__
JNIEXPORT void JNICALL MEDIAPIPE_SURFACE_OUTPUT_METHOD(nativeSetSurface)(
    JNIEnv* env, jobject thiz, jlong context, jlong packet, jobject surface);
#endif  // __ANDROID__

JNIEXPORT void JNICALL MEDIAPIPE_SURFACE_OUTPUT_METHOD(nativeSetEglSurface)(
    JNIEnv* env, jobject thiz, jlong context, jlong packet, jlong surface);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_SURFACE_OUTPUT_JNI_H_
