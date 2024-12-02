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
 * A callback that gets invoked when a texture is no longer in use.
 */
public interface TextureReleaseCallback {
  /**
   * Called when the texture has been released. The sync token can be used to ensure that the GPU is
   * done reading from it. Implementations of this interface should release the token once they are
   * done with it.
   */
  void release(GlSyncToken syncToken);
}
