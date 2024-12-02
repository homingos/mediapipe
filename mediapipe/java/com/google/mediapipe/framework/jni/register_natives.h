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

#ifndef JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_REGISTER_NATIVES_H_
#define JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_REGISTER_NATIVES_H_

#include <jni.h>

namespace mediapipe {
namespace android {
namespace registration {

void RegisterAllNatives(JNIEnv* env);

}  // namespace registration
}  // namespace android
}  // namespace mediapipe

#endif  // JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_REGISTER_NATIVES_H_
