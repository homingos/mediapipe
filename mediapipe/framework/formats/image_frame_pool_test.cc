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

#include "absl/memory/memory.h"
#include "mediapipe/framework/port/gmock.h"
#include "mediapipe/framework/port/gtest.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {
namespace {

using Pair = std::pair<int, int>;

constexpr int kWidth = 300;
constexpr int kHeight = 200;
constexpr ImageFormat::Format kFormat = ImageFormat::SRGBA;
constexpr int kKeepCount = 2;

class ImageFramePoolTest : public ::testing::Test {
 protected:
  ImageFramePoolTest() {
    pool_ = ImageFramePool::Create(kWidth, kHeight, kFormat, kKeepCount);
  }

  void SetUp() override {}

  std::shared_ptr<ImageFramePool> pool_;
};

TEST_F(ImageFramePoolTest, GetBuffer) {
  EXPECT_EQ(Pair(0, 0), pool_->GetInUseAndAvailableCounts());
  auto buffer = pool_->GetBuffer();
  EXPECT_EQ(Pair(1, 0), pool_->GetInUseAndAvailableCounts());
  buffer = nullptr;
  EXPECT_EQ(Pair(0, 1), pool_->GetInUseAndAvailableCounts());
  buffer = pool_->GetBuffer();
  EXPECT_EQ(Pair(1, 0), pool_->GetInUseAndAvailableCounts());
}

TEST_F(ImageFramePoolTest, GetMoreBuffers) {
  EXPECT_EQ(Pair(0, 0), pool_->GetInUseAndAvailableCounts());
  std::vector<ImageFrameSharedPtr> buffers;

  // Create kKeepCount + 1 buffers
  for (int i = 0; i <= kKeepCount; i++) {
    buffers.emplace_back(pool_->GetBuffer());
  }
  EXPECT_EQ(Pair(kKeepCount + 1, 0), pool_->GetInUseAndAvailableCounts());

  // Delete one
  buffers.resize(kKeepCount);
  EXPECT_EQ(Pair(kKeepCount, 0), pool_->GetInUseAndAvailableCounts());

  // Delete all
  buffers.resize(0);
  EXPECT_EQ(Pair(0, kKeepCount), pool_->GetInUseAndAvailableCounts());

  // Create one more
  buffers.emplace_back(pool_->GetBuffer());
  EXPECT_EQ(Pair(1, kKeepCount - 1), pool_->GetInUseAndAvailableCounts());
}

TEST_F(ImageFramePoolTest, DeleteNotLast) {
  EXPECT_EQ(Pair(0, 0), pool_->GetInUseAndAvailableCounts());
  std::vector<ImageFrameSharedPtr> buffers;

  // Create kKeepCount + 1 buffers
  for (int i = 0; i <= kKeepCount; i++) {
    buffers.emplace_back(pool_->GetBuffer());
  }
  EXPECT_EQ(Pair(kKeepCount + 1, 0), pool_->GetInUseAndAvailableCounts());

  // Delete second
  buffers.erase(buffers.begin() + 1);
  EXPECT_EQ(Pair(kKeepCount, 0), pool_->GetInUseAndAvailableCounts());

  // Delete first
  buffers.erase(buffers.begin());
  EXPECT_EQ(Pair(kKeepCount - 1, 1), pool_->GetInUseAndAvailableCounts());
}

TEST(ImageFrameBufferPoolStaticTest, BufferCanOutlivePool) {
  auto pool = ImageFramePool::Create(kWidth, kHeight, kFormat, kKeepCount);
  auto buffer = pool->GetBuffer();
  pool = nullptr;
  buffer = nullptr;
}

}  // anonymous namespace
}  // namespace mediapipe
