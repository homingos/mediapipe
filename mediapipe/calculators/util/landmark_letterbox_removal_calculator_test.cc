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

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/calculator_runner.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/port/gmock.h"
#include "mediapipe/framework/port/gtest.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status_matchers.h"
#include "mediapipe/framework/tool/validate_type.h"

namespace mediapipe {

constexpr char kLetterboxPaddingTag[] = "LETTERBOX_PADDING";
constexpr char kLandmarksTag[] = "LANDMARKS";

NormalizedLandmark CreateLandmark(float x, float y) {
  NormalizedLandmark landmark;
  landmark.set_x(x);
  landmark.set_y(y);
  return landmark;
}

CalculatorGraphConfig::Node GetDefaultNode() {
  return ParseTextProtoOrDie<CalculatorGraphConfig::Node>(R"pb(
    calculator: "LandmarkLetterboxRemovalCalculator"
    input_stream: "LANDMARKS:landmarks"
    input_stream: "LETTERBOX_PADDING:letterbox_padding"
    output_stream: "LANDMARKS:adjusted_landmarks"
  )pb");
}

TEST(LandmarkLetterboxRemovalCalculatorTest, PaddingLeftRight) {
  CalculatorRunner runner(GetDefaultNode());

  auto landmarks = absl::make_unique<NormalizedLandmarkList>();
  *landmarks->add_landmark() = CreateLandmark(0.5f, 0.5f);
  *landmarks->add_landmark() = CreateLandmark(0.2f, 0.2f);
  *landmarks->add_landmark() = CreateLandmark(0.7f, 0.7f);
  runner.MutableInputs()
      ->Tag(kLandmarksTag)
      .packets.push_back(
          Adopt(landmarks.release()).At(Timestamp::PostStream()));

  auto padding = absl::make_unique<std::array<float, 4>>(
      std::array<float, 4>{0.2f, 0.f, 0.3f, 0.f});
  runner.MutableInputs()
      ->Tag(kLetterboxPaddingTag)
      .packets.push_back(Adopt(padding.release()).At(Timestamp::PostStream()));

  MP_ASSERT_OK(runner.Run()) << "Calculator execution failed.";
  const std::vector<Packet>& output =
      runner.Outputs().Tag(kLandmarksTag).packets;
  ASSERT_EQ(1, output.size());
  const auto& output_landmarks = output[0].Get<NormalizedLandmarkList>();

  EXPECT_EQ(output_landmarks.landmark_size(), 3);

  EXPECT_THAT(output_landmarks.landmark(0).x(), testing::FloatNear(0.6f, 1e-5));
  EXPECT_THAT(output_landmarks.landmark(0).y(), testing::FloatNear(0.5f, 1e-5));
  EXPECT_THAT(output_landmarks.landmark(1).x(), testing::FloatNear(0.0f, 1e-5));
  EXPECT_THAT(output_landmarks.landmark(1).y(), testing::FloatNear(0.2f, 1e-5));
  EXPECT_THAT(output_landmarks.landmark(2).x(), testing::FloatNear(1.0f, 1e-5));
  EXPECT_THAT(output_landmarks.landmark(2).y(), testing::FloatNear(0.7f, 1e-5));
}

TEST(LandmarkLetterboxRemovalCalculatorTest, PaddingTopBottom) {
  CalculatorRunner runner(GetDefaultNode());

  auto landmarks = absl::make_unique<NormalizedLandmarkList>();
  NormalizedLandmark* landmark = landmarks->add_landmark();
  *landmark = CreateLandmark(0.5f, 0.5f);
  landmark = landmarks->add_landmark();
  *landmark = CreateLandmark(0.2f, 0.2f);
  landmark = landmarks->add_landmark();
  *landmark = CreateLandmark(0.7f, 0.7f);
  runner.MutableInputs()
      ->Tag(kLandmarksTag)
      .packets.push_back(
          Adopt(landmarks.release()).At(Timestamp::PostStream()));

  auto padding = absl::make_unique<std::array<float, 4>>(
      std::array<float, 4>{0.0f, 0.2f, 0.0f, 0.3f});
  runner.MutableInputs()
      ->Tag(kLetterboxPaddingTag)
      .packets.push_back(Adopt(padding.release()).At(Timestamp::PostStream()));

  MP_ASSERT_OK(runner.Run()) << "Calculator execution failed.";
  const std::vector<Packet>& output =
      runner.Outputs().Tag(kLandmarksTag).packets;
  ASSERT_EQ(1, output.size());
  const auto& output_landmarks = output[0].Get<NormalizedLandmarkList>();

  EXPECT_EQ(output_landmarks.landmark_size(), 3);

  EXPECT_THAT(output_landmarks.landmark(0).x(), testing::FloatNear(0.5f, 1e-5));
  EXPECT_THAT(output_landmarks.landmark(0).y(), testing::FloatNear(0.6f, 1e-5));
  EXPECT_THAT(output_landmarks.landmark(1).x(), testing::FloatNear(0.2f, 1e-5));
  EXPECT_THAT(output_landmarks.landmark(1).y(), testing::FloatNear(0.0f, 1e-5));
  EXPECT_THAT(output_landmarks.landmark(2).x(), testing::FloatNear(0.7f, 1e-5));
  EXPECT_THAT(output_landmarks.landmark(2).y(), testing::FloatNear(1.0f, 1e-5));
}

}  // namespace mediapipe
