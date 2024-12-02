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

package com.google.mediapipe.components;

import com.google.mediapipe.framework.TextureFrame;

/** Lightweight abstraction for an object that can receive video frames. */
public interface TextureFrameConsumer {
  /**
   * Called when a new {@link TextureFrame} is available.
   *
   * Important: implementations of this method should call frame.release().
   **/
  public abstract void onNewFrame(TextureFrame frame);
}
