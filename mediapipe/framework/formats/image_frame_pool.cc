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

#include "mediapipe/framework/formats/image_frame_pool.h"

#include "absl/synchronization/mutex.h"

namespace mediapipe {

ImageFramePool::ImageFramePool(int width, int height,
                               ImageFormat::Format format, int keep_count)
    : width_(width),
      height_(height),
      format_(format),
      keep_count_(keep_count) {}

ImageFrameSharedPtr ImageFramePool::GetBuffer() {
  std::unique_ptr<ImageFrame> buffer;

  {
    absl::MutexLock lock(&mutex_);
    if (available_.empty()) {
      // Fix alignment at 4 for best compatability with OpenGL.
      buffer = std::make_unique<ImageFrame>(
          format_, width_, height_, ImageFrame::kGlDefaultAlignmentBoundary);
      if (!buffer) return nullptr;
    } else {
      buffer = std::move(available_.back());
      available_.pop_back();
    }

    ++in_use_count_;
  }

  // Return a shared_ptr with a custom deleter that adds the buffer back
  // to our available list.
  std::weak_ptr<ImageFramePool> weak_pool(shared_from_this());
  return std::shared_ptr<ImageFrame>(buffer.release(),
                                     [weak_pool](ImageFrame* buf) {
                                       auto pool = weak_pool.lock();
                                       if (pool) {
                                         pool->Return(buf);
                                       } else {
                                         delete buf;
                                       }
                                     });
}

std::pair<int, int> ImageFramePool::GetInUseAndAvailableCounts() {
  absl::MutexLock lock(&mutex_);
  return {in_use_count_, available_.size()};
}

void ImageFramePool::Return(ImageFrame* buf) {
  std::vector<std::unique_ptr<ImageFrame>> trimmed;
  {
    absl::MutexLock lock(&mutex_);
    --in_use_count_;
    available_.emplace_back(buf);
    TrimAvailable(&trimmed);
  }
  // The trimmed buffers will be released without holding the lock.
}

void ImageFramePool::TrimAvailable(
    std::vector<std::unique_ptr<ImageFrame>>* trimmed) {
  int keep = std::max(keep_count_ - in_use_count_, 0);
  if (available_.size() > keep) {
    auto trim_it = std::next(available_.begin(), keep);
    if (trimmed) {
      std::move(trim_it, available_.end(), std::back_inserter(*trimmed));
    }
    available_.erase(trim_it, available_.end());
  }
}

}  // namespace mediapipe
