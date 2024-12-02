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

#include "mediapipe/java/com/google/mediapipe/framework/jni/android_asset_util_jni.h"

#include <memory>

#include "mediapipe/framework/port/logging.h"
#include "mediapipe/framework/port/singleton.h"
#include "mediapipe/java/com/google/mediapipe/framework/jni/jni_util.h"
#include "mediapipe/util/android/asset_manager_util.h"

JNIEXPORT jboolean JNICALL ANDROID_ASSET_UTIL_METHOD(
    nativeInitializeAssetManager)(JNIEnv* env, jclass clz,
                                  jobject android_context,
                                  jstring cache_dir_path) {
  mediapipe::AssetManager* asset_manager =
      Singleton<mediapipe::AssetManager>::get();
  return asset_manager->InitializeFromActivity(
      env, android_context,
      mediapipe::android::JStringToStdString(env, cache_dir_path));
}
