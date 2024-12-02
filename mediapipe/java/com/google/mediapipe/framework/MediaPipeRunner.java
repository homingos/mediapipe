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

import android.content.Context;

/** {@link MediaPipeRunner} is an abstract class for running MediaPipe graph in Android. */
public abstract class MediaPipeRunner extends Graph {
  protected Context context;

  public MediaPipeRunner(Context context) {
    // Creates a singleton AssetCache.
    AssetCache.create(context);
    this.context = context;
  }

  public void loadBinaryGraphFromAsset(String assetPath) {
    try {
      this.loadBinaryGraph(AssetCache.getAssetCache().getAbsolutePathFromAsset(assetPath));
    } catch (MediaPipeException e) {
      // TODO: Report this error from MediaPipe.
    }
  }

  /**
   * Starts running the graph.
   */
  public abstract void start();
  /**
   * Pauses a running graph.
   */
  public abstract void pause();
  /**
   * Resumes a paused graph.
   */
  public abstract void resume();
  /**
   * Stops the running graph and releases the resource. Call this in Activity onDestroy callback.
   */
  public abstract void release();
  /**
   * Stops the running graph and releases the resource. Call this in Activity onDestroy callback.
   *
   * <p>Like {@link #release()} but with a timeout. The default behavior is to call
   * {@link #release()}. The implementation can override this method if it cares about timing out
   * releasing resources.
   *
   * @param timeoutMillis the time it takes to force timeout from releasing mff context.
   */
  public void release(long timeoutMillis) {
    release();
  }
}
