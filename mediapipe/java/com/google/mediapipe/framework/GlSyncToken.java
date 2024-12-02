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
 * Represents a synchronization point for OpenGL operations. This can be needed when working with
 * multiple GL contexts.
 */
public interface GlSyncToken {
  /**
   * Waits until the GPU has executed all commands up to the sync point. This blocks the CPU, and
   * ensures the commands are complete from the point of view of all threads and contexts.
   */
  void waitOnCpu();

  /**
   * Ensures that the following commands on the current OpenGL context will not be executed until
   * the sync point has been reached. This does not block the CPU, and only affects the current
   * OpenGL context.
   */
  void waitOnGpu();

  /** Releases the underlying native object. */
  void release();

  /** Returns a handle to the underlying native object. For internal use. */
  long nativeToken();
}
