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

#include "mediapipe/util/tracking/image_util.h"

#include "mediapipe/framework/port/gtest.h"

namespace mediapipe {

template <int border>
void TestCopyBorder(cv::Mat* full_size) {
  full_size->setTo(0);

  const int width = full_size->cols - 2 * border;
  // Fill center with per pixel value [1, 2, 3].
  for (int y = border; y < full_size->rows - border; ++y) {
    float* row_ptr = full_size->ptr<float>(y);
    for (int x = border; x < full_size->cols - border; ++x) {
      int value = x - border;
      row_ptr[3 * x] = value + 0.1f;
      row_ptr[3 * x + 1] = value + 0.2f;
      row_ptr[3 * x + 2] = value + 0.3f;
    }
  }

  CopyMatBorder<float, border, 3>(full_size);

  // Main part should not be modified.
  for (int y = border; y < full_size->rows - border; ++y) {
    float* row_ptr = full_size->ptr<float>(y);
    for (int x = border; x < full_size->cols - border; ++x) {
      int value = x - border;
      EXPECT_EQ(row_ptr[3 * x], value + 0.1f);
      EXPECT_EQ(row_ptr[3 * x + 1], value + 0.2f);
      EXPECT_EQ(row_ptr[3 * x + 2], value + 0.3f);
    }
  }

  for (int y = 0; y < full_size->rows; ++y) {
    const float* row_ptr = full_size->ptr<float>(y);
    for (int x = 0; x < full_size->cols; ++x) {
      // Memory copy, expect exact floating point equality.
      int value = x - border;
      if (x < border) {
        // Cap to valid area.
        value = std::min(width - 1, (border - 1) - x);
      } else if (x >= full_size->cols - border) {
        // Last column minus distance from frame boundary, capped
        // to valid area.
        value = std::max(0, width - 1 - (x - full_size->cols + border));
      }
      EXPECT_EQ(row_ptr[3 * x], value + 0.1f);
      EXPECT_EQ(row_ptr[3 * x + 1], value + 0.2f);
      EXPECT_EQ(row_ptr[3 * x + 2], value + 0.3f);
    }
  }
}

TEST(PushPullFilteringTest, CopyBorder) {
  cv::Mat full_size(100, 50, CV_32FC3);
  TestCopyBorder<1>(&full_size);
  TestCopyBorder<2>(&full_size);
  TestCopyBorder<3>(&full_size);
  TestCopyBorder<4>(&full_size);
  TestCopyBorder<5>(&full_size);
}

TEST(PushPullFilteringTest, CopyBorderSmallFrame) {
  cv::Mat full_size(3, 3, CV_32FC3);
  TestCopyBorder<1>(&full_size);

  full_size.create(5, 5, CV_32FC3);
  TestCopyBorder<2>(&full_size);

  full_size.create(7, 7, CV_32FC3);
  TestCopyBorder<3>(&full_size);

  full_size.create(9, 9, CV_32FC3);
  TestCopyBorder<4>(&full_size);
}

}  // namespace mediapipe
