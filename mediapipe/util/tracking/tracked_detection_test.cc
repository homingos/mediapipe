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

#include "mediapipe/util/tracking/tracked_detection.h"

#include "mediapipe/framework/port/gtest.h"

namespace mediapipe {

using ::mediapipe::NormalizedRect;

const float kErrorMargin = 1e-4f;

TEST(TrackedDetectionTest, ConstructorWithoutBox) {
  TrackedDetection detection(0, 0);
  EXPECT_EQ(detection.unique_id(), 0);
  EXPECT_EQ(detection.previous_id(), -1);
  EXPECT_NEAR(detection.left(), 0.f, kErrorMargin);
  EXPECT_NEAR(detection.top(), 0.f, kErrorMargin);
  EXPECT_NEAR(detection.right(), 0.f, kErrorMargin);
  EXPECT_NEAR(detection.bottom(), 0.f, kErrorMargin);
}

TEST(TrackedDetectionTest, ConstructorWithAnOutOfBoundBox) {
  NormalizedRect box;
  box.set_x_center(-0.15);
  box.set_y_center(-0.15);
  box.set_height(0.1);
  box.set_width(0.1);
  TrackedDetection detection(0, 0, box);
  EXPECT_EQ(detection.unique_id(), 0);
  EXPECT_EQ(detection.previous_id(), -1);
  EXPECT_NEAR(detection.left(), -0.2f, kErrorMargin);
  EXPECT_NEAR(detection.top(), -0.2f, kErrorMargin);
  EXPECT_NEAR(detection.right(), -0.1f, kErrorMargin);
  EXPECT_NEAR(detection.bottom(), -0.1f, kErrorMargin);
  EXPECT_NEAR(detection.bounding_box().width(), 0.1f, kErrorMargin);
  EXPECT_NEAR(detection.bounding_box().height(), 0.1f, kErrorMargin);
}

TEST(TrackedDetectionTest, ConstructorWithAnOutOfBoundBox2) {
  NormalizedRect box;
  box.set_x_center(0.5);
  box.set_y_center(0.5);
  box.set_height(1.2);
  box.set_width(1.2);
  TrackedDetection detection(0, 0, box);
  EXPECT_EQ(detection.unique_id(), 0);
  EXPECT_EQ(detection.previous_id(), -1);
  EXPECT_NEAR(detection.left(), -0.1f, kErrorMargin);
  EXPECT_NEAR(detection.top(), -0.1f, kErrorMargin);
  EXPECT_NEAR(detection.right(), 1.1f, kErrorMargin);
  EXPECT_NEAR(detection.bottom(), 1.1f, kErrorMargin);
  EXPECT_NEAR(detection.bounding_box().width(), 1.2f, kErrorMargin);
  EXPECT_NEAR(detection.bounding_box().height(), 1.2f, kErrorMargin);
}

TEST(TrackedDetectionTest, CheckSameDetections) {
  NormalizedRect box0;
  box0.set_x_center(0.5);
  box0.set_y_center(0.5);
  box0.set_height(0.5);
  box0.set_width(0.5);
  NormalizedRect box1;
  box1.set_x_center(0.55);
  box1.set_y_center(0.5);
  box1.set_height(0.5);
  box1.set_width(0.5);
  NormalizedRect box2;
  box2.set_x_center(0.75);
  box2.set_y_center(0.55);
  box2.set_height(0.5);
  box2.set_width(0.5);
  TrackedDetection detection0(0, 0, box0);
  TrackedDetection detection1(1, 0, box1);
  TrackedDetection detection2(2, 0, box2);
  EXPECT_TRUE(detection0.IsSameAs(detection1));
  EXPECT_FALSE(detection0.IsSameAs(detection2));
}

TEST(TrackedDetectionTest, MergeLabels) {
  TrackedDetection detection0(0, 0);
  detection0.AddLabel("1", 1.f);
  detection0.AddLabel("2", 2.f);
  detection0.AddLabel("3", 3.f);

  TrackedDetection detection1(1, 0);
  detection0.AddLabel("1", 0.f);
  detection0.AddLabel("2", 3.f);
  detection0.AddLabel("4", 4.f);

  detection0.MergeLabelScore(detection1);
  const auto& labels = detection0.label_to_score_map();
  EXPECT_EQ(labels.find("1")->second, 1.f);
  EXPECT_EQ(labels.find("2")->second, 3.f);
  EXPECT_EQ(labels.find("3")->second, 3.f);
  EXPECT_EQ(labels.find("4")->second, 4.f);
}

}  // namespace mediapipe
