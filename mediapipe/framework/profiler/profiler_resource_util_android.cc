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

#include "mediapipe/framework/port/canonical_errors.h"
#include "mediapipe/framework/port/logging.h"
#include "mediapipe/framework/port/statusor.h"
#include "mediapipe/framework/profiler/profiler_resource_util.h"
#include "mediapipe/java/com/google/mediapipe/framework/jni/jni_util.h"

namespace mediapipe {

StatusOr<std::string> GetDefaultTraceLogDirectory() {
  // The path to external storage directory on a device doesn't change when an
  // application is running, hence can be stored as global state.
  static const StatusOr<std::string>* kExternalStorageDirectory = [] {
    StatusOr<std::string>* result = new StatusOr<std::string>();
    bool has_jvm = java::HasJavaVM();
    if (!has_jvm) {
      *result = absl::InternalError("JavaVM not available.");
      return result;
    }

    JNIEnv* env = java::GetJNIEnv();
    if (!env) {
      *result = absl::InternalError("JNIEnv not available.");
      return result;
    }

    // Get the class android.os.Environment.
    jclass environment_class = env->FindClass("android/os/Environment");

    // Get the id of the getExternalStorageDirectory method of the Android
    // Environment class.
    jmethodID environment_class_getExternalStorageDirectory =
        env->GetStaticMethodID(environment_class, "getExternalStorageDirectory",
                               "()Ljava/io/File;");

    // Call android.os.Environment.getExternalStorageDirectory().
    jobject storage_directory = env->CallStaticObjectMethod(
        environment_class, environment_class_getExternalStorageDirectory);

    // Get the class java.io.File.
    jclass file_class = env->FindClass("java/io/File");

    // Get the id of the getAbsolutePath method of the File class.
    jmethodID file_class_getAbsolutePath =
        env->GetMethodID(file_class, "getAbsolutePath", "()Ljava/lang/String;");

    // Call getAbsolutePath() on the storage_directory.
    jobject jpath =
        env->CallObjectMethod(storage_directory, file_class_getAbsolutePath);

    // Return the path of the external storage directory as an std::string
    // object.
    *result = std::string(
        android::JStringToStdString(env, static_cast<jstring>(jpath)));
    return result;
  }();

  return *kExternalStorageDirectory;
}

}  // namespace mediapipe
