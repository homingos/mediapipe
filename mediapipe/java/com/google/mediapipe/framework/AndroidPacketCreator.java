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

import android.graphics.Bitmap;
import android.graphics.PixelFormat;
import android.media.Image;
import com.google.mediapipe.framework.image.BitmapExtractor;
import com.google.mediapipe.framework.image.ByteBufferExtractor;
import com.google.mediapipe.framework.image.MPImage;
import com.google.mediapipe.framework.image.MPImageProperties;
import com.google.mediapipe.framework.image.MediaImageExtractor;
import java.nio.ByteBuffer;

// TODO: use Preconditions in this file.
/**
 * Android-specific subclass of PacketCreator.
 *
 * <p>See {@link PacketCreator} for general information.
 *
 * <p>This class contains methods that are Android-specific. You can (and should) use the base
 * PacketCreator on Android if you do not need any methods from this class.
 */
public class AndroidPacketCreator extends PacketCreator {
  public AndroidPacketCreator(Graph context) {
    super(context);
  }

  /** Creates a 3 channel RGB ImageFrame packet from a {@link Bitmap}. */
  public Packet createRgbImageFrame(Bitmap bitmap) {
    if (bitmap.getConfig() != Bitmap.Config.ARGB_8888) {
      throw new RuntimeException("bitmap must use ARGB_8888 config.");
    }
    return Packet.create(nativeCreateRgbImageFrame(mediapipeGraph.getNativeHandle(), bitmap));
  }

  /** Creates a 4 channel RGBA ImageFrame packet from a {@link Bitmap}. */
  public Packet createRgbaImageFrame(Bitmap bitmap) {
    if (bitmap.getConfig() != Bitmap.Config.ARGB_8888) {
      throw new RuntimeException("bitmap must use ARGB_8888 config.");
    }
    return Packet.create(nativeCreateRgbaImageFrame(mediapipeGraph.getNativeHandle(), bitmap));
  }

  /** Creates a 4 channel RGBA Image packet from a {@link Bitmap}. */
  public Packet createRgbaImage(Bitmap bitmap) {
    if (bitmap.getConfig() != Bitmap.Config.ARGB_8888) {
      throw new RuntimeException("bitmap must use ARGB_8888 config.");
    }
    return Packet.create(nativeCreateRgbaImage(mediapipeGraph.getNativeHandle(), bitmap));
  }

  /**
   * Creates a MediaPipe Image packet from a {@link MPImage}.
   *
   * <p>The ImageContainerType must be IMAGE_CONTAINER_BYTEBUFFER or IMAGE_CONTAINER_BITMAP.
   */
  public Packet createImage(MPImage image) {
    // TODO: Choose the best storage from multiple containers.
    MPImageProperties properties = image.getContainedImageProperties().get(0);
    if (properties.getStorageType() == MPImage.STORAGE_TYPE_BYTEBUFFER) {
      ByteBuffer buffer = ByteBufferExtractor.extract(image);
      int numChannels = 0;
      switch (properties.getImageFormat()) {
        case MPImage.IMAGE_FORMAT_RGBA:
          numChannels = 4;
          break;
        case MPImage.IMAGE_FORMAT_RGB:
          numChannels = 3;
          break;
        case MPImage.IMAGE_FORMAT_ALPHA:
          numChannels = 1;
          break;
        default: // fall out
      }
      if (numChannels == 0) {
        throw new UnsupportedOperationException(
            "Unsupported MediaPipe Image image format: " + properties.getImageFormat());
      }
      int width = image.getWidth();
      int height = image.getHeight();
      return createImage(buffer, width, height, numChannels);
    }
    if (properties.getStorageType() == MPImage.STORAGE_TYPE_BITMAP) {
      Bitmap bitmap = BitmapExtractor.extract(image);
      if (bitmap.getConfig() != Bitmap.Config.ARGB_8888) {
        throw new UnsupportedOperationException("bitmap must use ARGB_8888 config.");
      }
      return Packet.create(nativeCreateRgbaImage(mediapipeGraph.getNativeHandle(), bitmap));
    }
    if (properties.getStorageType() == MPImage.STORAGE_TYPE_MEDIA_IMAGE) {
      Image mediaImage = MediaImageExtractor.extract(image);
      if (mediaImage.getFormat() != PixelFormat.RGBA_8888) {
        throw new UnsupportedOperationException("Android media image must use RGBA_8888 config.");
      }
      return createImage(
          mediaImage.getPlanes()[0].getBuffer(),
          mediaImage.getWidth(),
          mediaImage.getHeight(),
          /* numChannels= */ 4);
    }
    // Unsupported type.
    throw new UnsupportedOperationException(
        "Unsupported Image container type: " + properties.getStorageType());
  }

  /**
   * Returns the native handle of a new internal::PacketWithContext object on success. Returns 0 on
   * failure.
   */
  private native long nativeCreateRgbImageFrame(long context, Bitmap bitmap);

  /**
   * Returns the native handle of a new internal::PacketWithContext object on success. Returns 0 on
   * failure.
   */
  private native long nativeCreateRgbaImageFrame(long context, Bitmap bitmap);

  /**
   * Returns the native handle of a new internal::PacketWithContext object on success. Returns 0 on
   * failure.
   */
  private native long nativeCreateRgbaImage(long context, Bitmap bitmap);
}
