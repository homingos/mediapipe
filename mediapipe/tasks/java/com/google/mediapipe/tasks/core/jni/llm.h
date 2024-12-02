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

#ifndef JAVA_COM_GOOGLE_MEDIAPIPE_TASKS_CORE_JNI_LLM_H_
#define JAVA_COM_GOOGLE_MEDIAPIPE_TASKS_CORE_JNI_LLM_H_

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#define JNI_METHOD(METHOD_NAME) \
  Java_com_google_mediapipe_tasks_core_LlmTaskRunner_##METHOD_NAME

/*
 * Class:     com_google_mediapipe_tasks_core_LlmTaskRunner
 * Method:    nativeCreateSession
 * Signature: ([B)J
 */
JNIEXPORT jlong JNICALL JNI_METHOD(nativeCreateSession)(JNIEnv *, jclass,
                                                        jbyteArray);

/*
 * Class:     com_google_mediapipe_tasks_core_LlmTaskRunner
 * Method:    nativeDeleteSession
 * Signature: (J)V
 */
JNIEXPORT void JNICALL JNI_METHOD(nativeDeleteSession)(JNIEnv *, jclass, jlong);

/*
 * Class:     com_google_mediapipe_tasks_core_LlmTaskRunner
 * Method:    nativePredictSync
 * Signature: (JLjava/lang/String;)[B
 */
JNIEXPORT jbyteArray JNICALL JNI_METHOD(nativePredictSync)(JNIEnv *, jclass,
                                                           jlong, jstring);

/*
 * Class:     com_google_mediapipe_tasks_core_LlmTaskRunner
 * Method:    nativeRegisterCallback
 * Signature: (Ljava/lang/Object;)Ljava/lang/Object
 */
JNIEXPORT jobject JNICALL JNI_METHOD(nativeRegisterCallback)(JNIEnv *, jclass,
                                                             jobject);

/*
 * Class:     com_google_mediapipe_tasks_core_LlmTaskRunner
 * Method:    nativeRemoveCallback
 * Signature: (Ljava/lang/Object;)V
 */
JNIEXPORT void JNICALL JNI_METHOD(nativeRemoveCallback)(JNIEnv *, jclass,
                                                        jobject);

/*
 * Class:     com_google_mediapipe_tasks_core_LlmTaskRunner
 * Method:    nativePredictAsync
 * Signature: (JLjava/lang/Object;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL JNI_METHOD(nativePredictAsync)(JNIEnv *, jclass, jlong,
                                                      jobject, jstring);

/*
 * Class:     com_google_mediapipe_tasks_core_LlmTaskRunner
 * Method:    nativeSizeInTokens
 * Signature: (JLjava/lang/String;)I
 */
JNIEXPORT jint JNICALL JNI_METHOD(nativeSizeInTokens)(JNIEnv *, jclass, jlong,
                                                      jstring);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // JAVA_COM_GOOGLE_MEDIAPIPE_TASKS_CORE_JNI_LLM_H_
