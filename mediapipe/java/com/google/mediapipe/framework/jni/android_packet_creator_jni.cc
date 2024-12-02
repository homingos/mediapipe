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

#include "mediapipe/java/com/google/mediapipe/framework/jni/android_packet_creator_jni.h"

#include <android/bitmap.h>

#include <cstring>
#include <memory>

#include "absl/log/absl_log.h"
#include "absl/memory/memory.h"
#include "mediapipe/framework/formats/image.h"
#include "mediapipe/framework/formats/image_format.pb.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/java/com/google/mediapipe/framework/jni/colorspace.h"
#include "mediapipe/java/com/google/mediapipe/framework/jni/graph.h"

namespace {

// Creates a new internal::PacketWithContext object, and returns the native
// handle.
int64_t CreatePacketWithContext(jlong context,
                                const mediapipe::Packet& packet) {
  mediapipe::android::Graph* mediapipe_graph =
      reinterpret_cast<mediapipe::android::Graph*>(context);
  return mediapipe_graph->WrapPacketIntoContext(packet);
}

// Create 3 or 4 channel 8-bit ImageFrame shared pointer from a Java Bitmap.
std::unique_ptr<mediapipe::ImageFrame> CreateImageFrameFromBitmap(
    JNIEnv* env, jobject bitmap, int width, int height, int stride,
    mediapipe::ImageFormat::Format format) {
  auto image_frame = std::make_unique<mediapipe::ImageFrame>(
      format, width, height,
      mediapipe::ImageFrame::kGlDefaultAlignmentBoundary);

  void* pixel_addr = nullptr;
  int result = AndroidBitmap_lockPixels(env, bitmap, &pixel_addr);
  if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
    ABSL_LOG(ERROR) << "AndroidBitmap_lockPixels() failed with result code "
                    << result;
    return nullptr;
  }

  if (format == mediapipe::ImageFormat::SRGBA) {
    const int64_t buffer_size = stride * height;
    if (buffer_size != image_frame->PixelDataSize()) {
      ABSL_LOG(ERROR) << "Bitmap stride: " << stride
                      << " times bitmap height: " << height
                      << " is not equal to the expected size: "
                      << image_frame->PixelDataSize();
      return nullptr;
    }
    std::memcpy(image_frame->MutablePixelData(), pixel_addr,
                image_frame->PixelDataSize());
  } else if (format == mediapipe::ImageFormat::SRGB) {
    if (stride != width * 4) {
      ABSL_LOG(ERROR) << "Bitmap stride: " << stride
                      << "is not equal to 4 times bitmap width: " << width;
      return nullptr;
    }
    const uint8_t* rgba_data = static_cast<uint8_t*>(pixel_addr);
    mediapipe::android::RgbaToRgb(rgba_data, stride, width, height,
                                  image_frame->MutablePixelData(),
                                  image_frame->WidthStep());
  } else {
    ABSL_LOG(ERROR) << "unsupported image format: " << format;
    return nullptr;
  }

  result = AndroidBitmap_unlockPixels(env, bitmap);
  if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
    ABSL_LOG(ERROR) << "AndroidBitmap_unlockPixels() failed with result code "
                    << result;
    return nullptr;
  }

  return image_frame;
}

}  // namespace

JNIEXPORT jlong JNICALL ANDROID_PACKET_CREATOR_METHOD(
    nativeCreateRgbImageFrame)(JNIEnv* env, jobject thiz, jlong context,
                               jobject bitmap) {
  AndroidBitmapInfo info;
  int result = AndroidBitmap_getInfo(env, bitmap, &info);
  if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
    ABSL_LOG(ERROR) << "AndroidBitmap_getInfo() failed with result code "
                    << result;
    return 0L;
  }

  auto image_frame =
      CreateImageFrameFromBitmap(env, bitmap, info.width, info.height,
                                 info.stride, mediapipe::ImageFormat::SRGB);
  if (nullptr == image_frame) return 0L;

  mediapipe::Packet packet = mediapipe::Adopt(image_frame.release());
  return CreatePacketWithContext(context, packet);
}

JNIEXPORT jlong JNICALL ANDROID_PACKET_CREATOR_METHOD(
    nativeCreateRgbaImageFrame)(JNIEnv* env, jobject thiz, jlong context,
                                jobject bitmap) {
  AndroidBitmapInfo info;
  int result = AndroidBitmap_getInfo(env, bitmap, &info);
  if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
    ABSL_LOG(ERROR) << "AndroidBitmap_getInfo() failed with result code "
                    << result;
    return 0L;
  }

  auto image_frame =
      CreateImageFrameFromBitmap(env, bitmap, info.width, info.height,
                                 info.stride, mediapipe::ImageFormat::SRGBA);
  if (nullptr == image_frame) return 0L;

  mediapipe::Packet packet = mediapipe::Adopt(image_frame.release());
  return CreatePacketWithContext(context, packet);
}

JNIEXPORT jlong JNICALL ANDROID_PACKET_CREATOR_METHOD(nativeCreateRgbaImage)(
    JNIEnv* env, jobject thiz, jlong context, jobject bitmap) {
  AndroidBitmapInfo info;
  int result = AndroidBitmap_getInfo(env, bitmap, &info);
  if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
    ABSL_LOG(ERROR) << "AndroidBitmap_getInfo() failed with result code "
                    << result;
    return 0L;
  }

  auto image_frame =
      CreateImageFrameFromBitmap(env, bitmap, info.width, info.height,
                                 info.stride, mediapipe::ImageFormat::SRGBA);
  if (nullptr == image_frame) return 0L;

  mediapipe::Packet packet =
      mediapipe::MakePacket<mediapipe::Image>(std::move(image_frame));
  return CreatePacketWithContext(context, packet);
}
