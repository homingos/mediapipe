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
import android.content.res.AssetManager;
import com.google.common.io.ByteStreams;
import java.io.IOException;
import java.io.InputStream;

/**
 * Helper methods for handling Android assets.
 */
public final class AndroidAssetUtil {
  /**
   * Returns an asset's contents as a byte array. This is meant to be used in combination with
   * {@link Graph#loadBinaryGraph}.
   *
   * @param assetName The name of an asset, same as in {@link AssetManager#open(String)}.
   */
  public static byte[] getAssetBytes(AssetManager assets, String assetName) {
    byte[] assetData;
    try {
      InputStream stream = assets.open(assetName);
      assetData = ByteStreams.toByteArray(stream);
      stream.close();
    } catch (IOException e) {
      throw new RuntimeException(e);
    }
    return assetData;
  }

  /**
   * Initializes the native asset manager, which is used by native code to access assets directly.
   *
   * <p>Note: native AssetManager is a singleton, so this initialization should happen once.
   *
   * <p>Note: native AssetManager, if initialized, is used by MediaPipe to load assets automatically
   * (e.g. calculator calling GetResourceContents will also try loading from the assets).
   *
   * <p>Note: alternatively, you can use {@link AssetCache} to extract assets to app cache folder to
   * access them by regular file paths (beware that cached resources may require versioning).
   */
  public static synchronized boolean initializeNativeAssetManager(Context androidContext) {
    return nativeInitializeAssetManager(
        androidContext, androidContext.getCacheDir().getAbsolutePath());
  }

  private static native boolean nativeInitializeAssetManager(
      Context androidContext, String cacheDirPath);

  private AndroidAssetUtil() {}
}
