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

#include "mediapipe/java/com/google/mediapipe/framework/jni/graph_profiler_jni.h"

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/calculator_profile.pb.h"

JNIEXPORT void JNICALL GRAPH_PROFILER_METHOD(nativeReset)(JNIEnv* env,
                                                          jobject thiz,
                                                          jlong handle) {
  mediapipe::ProfilingContext* profiling_context =
      reinterpret_cast<mediapipe::ProfilingContext*>(handle);
  profiling_context->Reset();
}

JNIEXPORT void JNICALL GRAPH_PROFILER_METHOD(nativePause)(JNIEnv* env,
                                                          jobject thiz,
                                                          jlong handle) {
  mediapipe::ProfilingContext* profiling_context =
      reinterpret_cast<mediapipe::ProfilingContext*>(handle);
  profiling_context->Pause();
}

JNIEXPORT void JNICALL GRAPH_PROFILER_METHOD(nativeResume)(JNIEnv* env,
                                                           jobject thiz,
                                                           jlong handle) {
  mediapipe::ProfilingContext* profiling_context =
      reinterpret_cast<mediapipe::ProfilingContext*>(handle);
  profiling_context->Resume();
}

JNIEXPORT jobjectArray JNICALL GRAPH_PROFILER_METHOD(
    nativeGetCalculatorProfiles)(JNIEnv* env, jobject thiz, jlong handle) {
  mediapipe::ProfilingContext* profiling_context =
      reinterpret_cast<mediapipe::ProfilingContext*>(handle);

  std::vector<mediapipe::CalculatorProfile> profiles_vec;
  if (profiling_context->GetCalculatorProfiles(&profiles_vec) !=
      absl::OkStatus()) {
    return nullptr;
  }
  int num_profiles = profiles_vec.size();
  if (num_profiles == 0) {
    return nullptr;
  }

  // TODO: move to register natives.
  jclass byte_array_cls = env->FindClass("[B");
  jobjectArray profiles =
      env->NewObjectArray(num_profiles, byte_array_cls, nullptr);
  env->DeleteLocalRef(byte_array_cls);
  for (int i = 0; i < num_profiles; i++) {
    const auto& profile = profiles_vec[i];
    int size = profile.ByteSize();

    jbyteArray byteArray = env->NewByteArray(size);
    jbyte* byteArrayBuffer = env->GetByteArrayElements(byteArray, nullptr);
    profile.SerializeToArray(byteArrayBuffer, size);
    env->ReleaseByteArrayElements(byteArray, byteArrayBuffer, 0);

    env->SetObjectArrayElement(profiles, i, byteArray);
    env->DeleteLocalRef(byteArray);
  }

  return profiles;
}
