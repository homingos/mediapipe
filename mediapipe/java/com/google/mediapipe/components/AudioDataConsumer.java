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

import android.media.AudioFormat;
import java.nio.ByteBuffer;

/** Lightweight abstraction for an object that can receive audio data. */
public interface AudioDataConsumer {
  /**
   * Called when a new audio data buffer is available. Note, for consistency, the ByteBuffer used in
   * AudioDataConsumer has to use AudioFormat.ENCODING_PCM_16BIT, 2 bytes per sample, FILLED with
   * ByteOrder.LITTLE_ENDIAN, which is ByteOrder.nativeOrder() on Android
   * (https://developer.android.com/ndk/guides/abis.html).
   */
  public abstract void onNewAudioData(
      ByteBuffer audioData, long timestampMicros, AudioFormat audioFormat);
}
