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

#include "absl/container/btree_set.h"
#include "absl/flags/flag.h"
#include "absl/strings/string_view.h"
#include "mediapipe/examples/desktop/autoflip/calculators/shot_boundary_calculator.pb.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/calculator_runner.h"
#include "mediapipe/framework/deps/file_path.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/port/gmock.h"
#include "mediapipe/framework/port/gtest.h"
#include "mediapipe/framework/port/opencv_core_inc.h"
#include "mediapipe/framework/port/opencv_imgcodecs_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/port/status_matchers.h"

using mediapipe::CalculatorGraphConfig;
using mediapipe::CalculatorRunner;
using mediapipe::ImageFormat;
using mediapipe::ImageFrame;
using mediapipe::PacketTypeSet;

namespace mediapipe {
namespace autoflip {
namespace {

constexpr char kIsShotChangeTag[] = "IS_SHOT_CHANGE";
constexpr char kVideoTag[] = "VIDEO";

const char kConfig[] = R"(
    calculator: "ShotBoundaryCalculator"
    input_stream: "VIDEO:camera_frames"
    output_stream: "IS_SHOT_CHANGE:is_shot"
    )";
const int kTestFrameWidth = 640;
const int kTestFrameHeight = 480;

void AddFrames(const int number_of_frames,
               const absl::btree_set<int>& skip_frames,
               CalculatorRunner* runner) {
  cv::Mat image =
      cv::imread(file::JoinPath("./",
                                "/mediapipe/examples/desktop/"
                                "autoflip/calculators/testdata/dino.jpg"));

  for (int i = 0; i < number_of_frames; i++) {
    auto input_frame = ::absl::make_unique<ImageFrame>(
        ImageFormat::SRGB, kTestFrameWidth, kTestFrameHeight);
    cv::Mat input_mat = mediapipe::formats::MatView(input_frame.get());
    input_mat.setTo(cv::Scalar(0, 0, 0));
    cv::Mat sub_image =
        image(cv::Rect(i, i, kTestFrameWidth, kTestFrameHeight));
    cv::Mat frame_area =
        input_mat(cv::Rect(0, 0, sub_image.cols, sub_image.rows));
    if (skip_frames.count(i) < 1) {
      sub_image.copyTo(frame_area);
    }
    runner->MutableInputs()->Tag(kVideoTag).packets.push_back(
        Adopt(input_frame.release()).At(Timestamp(i * 1000000)));
  }
}

void CheckOutput(const int number_of_frames,
                 const absl::btree_set<int>& shot_frames,
                 const std::vector<Packet>& output_packets) {
  ASSERT_EQ(number_of_frames, output_packets.size());
  for (int i = 0; i < number_of_frames; i++) {
    if (shot_frames.count(i) < 1) {
      EXPECT_FALSE(output_packets[i].Get<bool>());
    } else {
      EXPECT_TRUE(output_packets[i].Get<bool>());
    }
  }
}

TEST(ShotBoundaryCalculatorTest, NoShotChange) {
  CalculatorGraphConfig::Node node =
      ParseTextProtoOrDie<CalculatorGraphConfig::Node>(kConfig);
  node.mutable_options()
      ->MutableExtension(ShotBoundaryCalculatorOptions::ext)
      ->set_output_only_on_change(false);
  auto runner = ::absl::make_unique<CalculatorRunner>(node);

  AddFrames(10, {}, runner.get());
  MP_ASSERT_OK(runner->Run());
  CheckOutput(10, {}, runner->Outputs().Tag(kIsShotChangeTag).packets);
}

TEST(ShotBoundaryCalculatorTest, ShotChangeSingle) {
  CalculatorGraphConfig::Node node =
      ParseTextProtoOrDie<CalculatorGraphConfig::Node>(kConfig);
  node.mutable_options()
      ->MutableExtension(ShotBoundaryCalculatorOptions::ext)
      ->set_output_only_on_change(false);
  auto runner = ::absl::make_unique<CalculatorRunner>(node);

  AddFrames(20, {10}, runner.get());
  MP_ASSERT_OK(runner->Run());
  CheckOutput(20, {10}, runner->Outputs().Tag(kIsShotChangeTag).packets);
}

TEST(ShotBoundaryCalculatorTest, ShotChangeDouble) {
  CalculatorGraphConfig::Node node =
      ParseTextProtoOrDie<CalculatorGraphConfig::Node>(kConfig);
  node.mutable_options()
      ->MutableExtension(ShotBoundaryCalculatorOptions::ext)
      ->set_output_only_on_change(false);
  auto runner = ::absl::make_unique<CalculatorRunner>(node);

  AddFrames(20, {14, 17}, runner.get());
  MP_ASSERT_OK(runner->Run());
  CheckOutput(20, {14, 17}, runner->Outputs().Tag(kIsShotChangeTag).packets);
}

TEST(ShotBoundaryCalculatorTest, ShotChangeFiltered) {
  CalculatorGraphConfig::Node node =
      ParseTextProtoOrDie<CalculatorGraphConfig::Node>(kConfig);
  node.mutable_options()
      ->MutableExtension(ShotBoundaryCalculatorOptions::ext)
      ->set_min_shot_span(5);
  node.mutable_options()
      ->MutableExtension(ShotBoundaryCalculatorOptions::ext)
      ->set_output_only_on_change(false);

  auto runner = ::absl::make_unique<CalculatorRunner>(node);

  AddFrames(24, {16, 19}, runner.get());
  MP_ASSERT_OK(runner->Run());
  CheckOutput(24, {16}, runner->Outputs().Tag(kIsShotChangeTag).packets);
}

TEST(ShotBoundaryCalculatorTest, ShotChangeSingleOnOnChange) {
  CalculatorGraphConfig::Node node =
      ParseTextProtoOrDie<CalculatorGraphConfig::Node>(kConfig);
  node.mutable_options()
      ->MutableExtension(ShotBoundaryCalculatorOptions::ext)
      ->set_output_only_on_change(true);
  auto runner = ::absl::make_unique<CalculatorRunner>(node);

  AddFrames(20, {15}, runner.get());
  MP_ASSERT_OK(runner->Run());
  auto output_packets = runner->Outputs().Tag(kIsShotChangeTag).packets;
  ASSERT_EQ(output_packets.size(), 1);
  ASSERT_EQ(output_packets[0].Get<bool>(), true);
  ASSERT_EQ(output_packets[0].Timestamp().Value(), 15000000);
}

}  // namespace
}  // namespace autoflip
}  // namespace mediapipe
