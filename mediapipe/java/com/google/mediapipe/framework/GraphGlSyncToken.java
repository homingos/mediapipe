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

package com.google.mediapipe.framework;

/**
 * Represents a synchronization point for OpenGL operations. It can be used to wait until the GPU
 * has reached the specified point in the sequence of commands it is executing. This can be
 * necessary when working with multiple GL contexts.
 */
public final class GraphGlSyncToken implements GlSyncToken {
  private long token;

  @Override
  public void waitOnCpu() {
    if (token != 0) {
      nativeWaitOnCpu(token);
    }
  }

  @Override
  public void waitOnGpu() {
    if (token != 0) {
      nativeWaitOnGpu(token);
    }
  }

  @Override
  public void release() {
    if (token != 0) {
      nativeRelease(token);
      token = 0;
    }
  }

  @Override
  public long nativeToken() {
    return token;
  }

  public GraphGlSyncToken(long token) {
    this.token = token;
  }

  private static native void nativeWaitOnCpu(long token);

  private static native void nativeWaitOnGpu(long token);

  private static native void nativeRelease(long token);
}
