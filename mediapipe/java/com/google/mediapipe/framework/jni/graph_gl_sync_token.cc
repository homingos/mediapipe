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

#include "mediapipe/java/com/google/mediapipe/framework/jni/graph_gl_sync_token.h"

#include <memory>

#include "mediapipe/framework/port/logging.h"
#include "mediapipe/gpu/gl_context.h"
#include "mediapipe/java/com/google/mediapipe/framework/jni/jni_util.h"

JNIEXPORT void JNICALL GRAPH_GL_SYNC_TOKEN_METHOD(nativeWaitOnCpu)(
    JNIEnv* env, jclass cls, jlong syncToken) {
  mediapipe::GlSyncToken& token =
      *reinterpret_cast<mediapipe::GlSyncToken*>(syncToken);
  token->Wait();
}

JNIEXPORT void JNICALL GRAPH_GL_SYNC_TOKEN_METHOD(nativeWaitOnGpu)(
    JNIEnv* env, jclass cls, jlong syncToken) {
  mediapipe::GlSyncToken& token =
      *reinterpret_cast<mediapipe::GlSyncToken*>(syncToken);
  token->WaitOnGpu();
}

JNIEXPORT void JNICALL GRAPH_GL_SYNC_TOKEN_METHOD(nativeRelease)(
    JNIEnv* env, jclass cls, jlong syncToken) {
  delete reinterpret_cast<mediapipe::GlSyncToken*>(syncToken);
}
