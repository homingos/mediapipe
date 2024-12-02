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
 * Interface for a video frame that can be accessed as a texture.
 *
 * <p>This interface defines a producer/consumer relationship between the component that originates
 * the TextureFrame and the component that receives it. The consumer <b>must</b> call {@link
 * #release()} when it is done using the frame. This gives the producer the opportunity to recycle
 * the resource.
 *
 * <p>When your application sends a TextureFrame into a MediaPipe graph, the application is the
 * producer and MediaPipe is the consumer. MediaPipe will call the release() method when all copies
 * of the packet holding the texture have been destroyed.
 *
 * <p>When MediaPipe sends a TextureFrame to the application, MediaPipe is the producer and the
 * application is the consumer. The application should call the release() method.
 *
 * <p>You can also send a TextureFrame from a component of your application to another. In this
 * case, the receiving component is the consumer, and should call release(). This can be useful, for
 * instance, if your application requires a "raw" mode where frames are sent directly from the video
 * source to the renderer, bypassing MediaPipe.
 */
public interface TextureFrame extends TextureReleaseCallback {
  /** The OpenGL name of the texture. */
  int getTextureName();

  /** Width of the frame in pixels. */
  int getWidth();

  /** Height of the frame in pixels. */
  int getHeight();

  /** The presentation time of the frame in microseconds **/
  long getTimestamp();

  /**
   * The consumer that receives this TextureFrame must call this method to inform the provider that
   * it is done with it.
   */
  void release();

  /**
   * If this texture is provided to MediaPipe, this method will be called when it is released. The
   * {@link GlSyncToken} can be used to wait for the GPU to be entirely done reading the texture.
   */
  @Override
  void release(GlSyncToken syncToken);

  /**
   * If this method returns true, this object supports the retain method, and can be used with
   * multiple consumers. Call retain for each additional consumer beyond the first; each consumer
   * should call release.
   */
  default boolean supportsRetain() {
    return false;
  }

  /** Increments the reference count. Only available with some implementations of TextureFrame. */
  default void retain() {
    throw new UnsupportedOperationException();
  }
}
