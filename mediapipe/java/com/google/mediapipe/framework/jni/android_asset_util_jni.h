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

#ifndef JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_ANDROID_ASSET_UTIL_JNI_H_
#define JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_ANDROID_ASSET_UTIL_JNI_H_

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#define ANDROID_ASSET_UTIL_METHOD(METHOD_NAME) \
  Java_com_google_mediapipe_framework_AndroidAssetUtil_##METHOD_NAME

JNIEXPORT jboolean JNICALL ANDROID_ASSET_UTIL_METHOD(
    nativeInitializeAssetManager)(JNIEnv* env, jclass clz,
                                  jobject android_context,
                                  jstring cache_dir_path);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_ANDROID_ASSET_UTIL_JNI_H_
