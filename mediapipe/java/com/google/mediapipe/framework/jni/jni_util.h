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

#ifndef JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_JNI_UTIL_H_
#define JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_JNI_UTIL_H_

#include <jni.h>

#include <string>

#include "mediapipe/framework/port/status.h"

namespace mediapipe {

namespace android {

std::string JStringToStdString(JNIEnv* env, jstring jstr);

std::vector<std::string> JavaListToStdStringVector(JNIEnv* env, jobject from);

// Creates a java MediaPipeException object for a absl::Status.
jthrowable CreateMediaPipeException(JNIEnv* env, absl::Status status);

// Throws a MediaPipeException for any non-ok absl::Status.
// Note that the exception is thrown after execution returns to Java.
bool ThrowIfError(JNIEnv* env, absl::Status status);

// The Jni ids for Java class SerializedMessage.
class SerializedMessageIds {
 public:
  SerializedMessageIds(JNIEnv* env, jobject data);
  jclass j_class;
  jfieldID type_name_id;
  jfieldID value_id;
};

}  // namespace android

namespace java {

// Sets the global Java VM instance, if it is not set yet.
// Returns true on success.
bool SetJavaVM(JNIEnv* env);

// Determines if the global Java VM instance is available.
bool HasJavaVM();

// Returns the current JNI environment.
JNIEnv* GetJNIEnv();

}  // namespace java

}  // namespace mediapipe

#endif  // JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_JNI_UTIL_H_
