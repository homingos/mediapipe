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

#include "mediapipe/examples/desktop/autoflip/quality/visual_scorer.h"

#include "mediapipe/framework/port/gmock.h"
#include "mediapipe/framework/port/gtest.h"
#include "mediapipe/framework/port/opencv_core_inc.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status_matchers.h"

namespace mediapipe {
namespace autoflip {
namespace {

TEST(VisualScorerTest, ScoresArea) {
  cv::Mat image_mat(200, 200, CV_8UC3);
  SalientRegion region = ParseTextProtoOrDie<SalientRegion>(
      R"pb(location { x: 10 y: 10 width: 100 height: 100 })pb");

  VisualScorerOptions options = ParseTextProtoOrDie<VisualScorerOptions>(
      R"pb(area_weight: 1.0 sharpness_weight: 0 colorfulness_weight: 0)pb");
  VisualScorer scorer(options);
  float score = 0.0;
  MP_EXPECT_OK(scorer.CalculateScore(image_mat, region, &score));
  EXPECT_EQ(0.25, score);  // (100 * 100) / (200 * 200).
}

TEST(VisualScorerTest, ScoresSharpness) {
  SalientRegion region = ParseTextProtoOrDie<SalientRegion>(
      R"pb(location { x: 10 y: 10 width: 100 height: 100 })pb");

  VisualScorerOptions options = ParseTextProtoOrDie<VisualScorerOptions>(
      R"pb(area_weight: 0 sharpness_weight: 1.0 colorfulness_weight: 0)pb");
  VisualScorer scorer(options);

  // Compute the score of an empty image and an image with a rectangle.
  cv::Mat image_mat(200, 200, CV_8UC3);
  image_mat.setTo(cv::Scalar(0, 0, 0));
  float score_rect = 0;
  auto status = scorer.CalculateScore(image_mat, region, &score_rect);
  EXPECT_EQ(status.code(), StatusCode::kInvalidArgument);
}

TEST(VisualScorerTest, ScoresColorfulness) {
  SalientRegion region = ParseTextProtoOrDie<SalientRegion>(
      R"pb(location { x: 10 y: 10 width: 50 height: 150 })pb");

  VisualScorerOptions options = ParseTextProtoOrDie<VisualScorerOptions>(
      R"pb(area_weight: 0 sharpness_weight: 0 colorfulness_weight: 1.0)pb");
  VisualScorer scorer(options);

  // Compute the scores of images with 1, 2 and 3 colors.
  cv::Mat image_mat(200, 200, CV_8UC3);
  image_mat.setTo(cv::Scalar(0, 0, 255));
  float score_1c = 0, score_2c = 0, score_3c = 0;
  MP_EXPECT_OK(scorer.CalculateScore(image_mat, region, &score_1c));
  image_mat(cv::Rect(30, 30, 20, 20)).setTo(cv::Scalar(128, 0, 0));
  MP_EXPECT_OK(scorer.CalculateScore(image_mat, region, &score_2c));
  image_mat(cv::Rect(50, 50, 20, 20)).setTo(cv::Scalar(255, 128, 0));
  MP_EXPECT_OK(scorer.CalculateScore(image_mat, region, &score_3c));
  // Images with more colors should have a higher score.
  EXPECT_LT(score_1c, score_2c);
  EXPECT_LT(score_2c, score_3c);
}

}  // namespace
}  // namespace autoflip
}  // namespace mediapipe
