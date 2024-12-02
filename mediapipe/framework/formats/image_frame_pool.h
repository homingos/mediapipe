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

// Consider this file an implementation detail. None of this is part of the
// public API.

#ifndef MEDIAPIPE_FRAMEWORK_FORMATS_IMAGE_FRAME_POOL_H_
#define MEDIAPIPE_FRAMEWORK_FORMATS_IMAGE_FRAME_POOL_H_

#include <utility>
#include <vector>

#include "absl/synchronization/mutex.h"
#include "mediapipe/framework/formats/image_frame.h"

namespace mediapipe {

using ImageFrameSharedPtr = std::shared_ptr<ImageFrame>;

class ImageFramePool : public std::enable_shared_from_this<ImageFramePool> {
 public:
  // Creates a pool. This pool will manage buffers of the specified dimensions,
  // and will keep keep_count buffers around for reuse.
  // We enforce creation as a shared_ptr so that we can use a weak reference in
  // the buffers' deleters.
  static std::shared_ptr<ImageFramePool> Create(int width, int height,
                                                ImageFormat::Format format,
                                                int keep_count) {
    return std::shared_ptr<ImageFramePool>(
        new ImageFramePool(width, height, format, keep_count));
  }

  // Obtains a buffers. May either be reused or created anew.
  ImageFrameSharedPtr GetBuffer();

  int width() const { return width_; }
  int height() const { return height_; }
  ImageFormat::Format format() const { return format_; }

  // This method is meant for testing.
  std::pair<int, int> GetInUseAndAvailableCounts();

 private:
  ImageFramePool(int width, int height, ImageFormat::Format format,
                 int keep_count);

  // Return a buffer to the pool.
  void Return(ImageFrame* buf);

  // If the total number of buffers is greater than keep_count, destroys any
  // surplus buffers that are no longer in use.
  void TrimAvailable(std::vector<std::unique_ptr<ImageFrame>>* trimmed)
      ABSL_EXCLUSIVE_LOCKS_REQUIRED(mutex_);

  const int width_;
  const int height_;
  const ImageFormat::Format format_;
  const int keep_count_;

  absl::Mutex mutex_;
  int in_use_count_ ABSL_GUARDED_BY(mutex_) = 0;
  std::vector<std::unique_ptr<ImageFrame>> available_ ABSL_GUARDED_BY(mutex_);
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_FORMATS_IMAGE_FRAME_POOL_H_
