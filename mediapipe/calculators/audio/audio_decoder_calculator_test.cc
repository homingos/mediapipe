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

#include "absl/flags/flag.h"
#include "mediapipe/framework/calculator_runner.h"
#include "mediapipe/framework/deps/file_path.h"
#include "mediapipe/framework/formats/time_series_header.pb.h"
#include "mediapipe/framework/port/gmock.h"
#include "mediapipe/framework/port/gtest.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status_matchers.h"
#include "mediapipe/framework/tool/test_util.h"

namespace mediapipe {
namespace {

constexpr char kTestPackageRoot[] = "mediapipe/calculators/audio";

TEST(AudioDecoderCalculatorTest, TestWAV) {
  CalculatorGraphConfig::Node node_config =
      ParseTextProtoOrDie<CalculatorGraphConfig::Node>(R"pb(
        calculator: "AudioDecoderCalculator"
        input_side_packet: "INPUT_FILE_PATH:input_file_path"
        output_stream: "AUDIO:audio"
        output_stream: "AUDIO_HEADER:audio_header"
        node_options {
          [type.googleapis.com/mediapipe.AudioDecoderOptions]: {
            audio_stream { stream_index: 0 }
          }
        })pb");
  CalculatorRunner runner(node_config);
  runner.MutableSidePackets()->Tag("INPUT_FILE_PATH") = MakePacket<std::string>(
      file::JoinPath(GetTestDataDir(kTestPackageRoot),
                     "sine_wave_1k_44100_mono_2_sec_wav.audio"));
  MP_ASSERT_OK(runner.Run());
  MP_EXPECT_OK(runner.Outputs()
                   .Tag("AUDIO_HEADER")
                   .header.ValidateAsType<mediapipe::TimeSeriesHeader>());
  const mediapipe::TimeSeriesHeader& header =
      runner.Outputs()
          .Tag("AUDIO_HEADER")
          .header.Get<mediapipe::TimeSeriesHeader>();
  EXPECT_EQ(44100, header.sample_rate());
  EXPECT_EQ(1, header.num_channels());
  EXPECT_TRUE(runner.Outputs().Tag("AUDIO").packets.size() >=
              std::ceil(44100.0 * 2 / 2048));
}

TEST(AudioDecoderCalculatorTest, Test48KWAV) {
  CalculatorGraphConfig::Node node_config =
      ParseTextProtoOrDie<CalculatorGraphConfig::Node>(R"pb(
        calculator: "AudioDecoderCalculator"
        input_side_packet: "INPUT_FILE_PATH:input_file_path"
        output_stream: "AUDIO:audio"
        output_stream: "AUDIO_HEADER:audio_header"
        node_options {
          [type.googleapis.com/mediapipe.AudioDecoderOptions]: {
            audio_stream { stream_index: 0 }
          }
        })pb");
  CalculatorRunner runner(node_config);
  runner.MutableSidePackets()->Tag("INPUT_FILE_PATH") = MakePacket<std::string>(
      file::JoinPath(GetTestDataDir(kTestPackageRoot),
                     "sine_wave_1k_48000_stereo_2_sec_wav.audio"));
  MP_ASSERT_OK(runner.Run());
  MP_EXPECT_OK(runner.Outputs()
                   .Tag("AUDIO_HEADER")
                   .header.ValidateAsType<mediapipe::TimeSeriesHeader>());
  const mediapipe::TimeSeriesHeader& header =
      runner.Outputs()
          .Tag("AUDIO_HEADER")
          .header.Get<mediapipe::TimeSeriesHeader>();
  EXPECT_EQ(48000, header.sample_rate());
  EXPECT_EQ(2, header.num_channels());
  EXPECT_TRUE(runner.Outputs().Tag("AUDIO").packets.size() >=
              std::ceil(48000.0 * 2 / 1024));
}

TEST(AudioDecoderCalculatorTest, TestMP3) {
  CalculatorGraphConfig::Node node_config =
      ParseTextProtoOrDie<CalculatorGraphConfig::Node>(R"pb(
        calculator: "AudioDecoderCalculator"
        input_side_packet: "INPUT_FILE_PATH:input_file_path"
        output_stream: "AUDIO:audio"
        output_stream: "AUDIO_HEADER:audio_header"
        node_options {
          [type.googleapis.com/mediapipe.AudioDecoderOptions]: {
            audio_stream { stream_index: 0 }
          }
        })pb");
  CalculatorRunner runner(node_config);
  runner.MutableSidePackets()->Tag("INPUT_FILE_PATH") = MakePacket<std::string>(
      file::JoinPath(GetTestDataDir(kTestPackageRoot),
                     "sine_wave_1k_44100_stereo_2_sec_mp3.audio"));
  MP_ASSERT_OK(runner.Run());
  MP_EXPECT_OK(runner.Outputs()
                   .Tag("AUDIO_HEADER")
                   .header.ValidateAsType<mediapipe::TimeSeriesHeader>());
  const mediapipe::TimeSeriesHeader& header =
      runner.Outputs()
          .Tag("AUDIO_HEADER")
          .header.Get<mediapipe::TimeSeriesHeader>();
  EXPECT_EQ(44100, header.sample_rate());
  EXPECT_EQ(2, header.num_channels());
  EXPECT_TRUE(runner.Outputs().Tag("AUDIO").packets.size() >=
              std::ceil(44100.0 * 2 / 1152));
}

TEST(AudioDecoderCalculatorTest, TestAAC) {
  CalculatorGraphConfig::Node node_config =
      ParseTextProtoOrDie<CalculatorGraphConfig::Node>(R"pb(
        calculator: "AudioDecoderCalculator"
        input_side_packet: "INPUT_FILE_PATH:input_file_path"
        output_stream: "AUDIO:audio"
        output_stream: "AUDIO_HEADER:audio_header"
        node_options {
          [type.googleapis.com/mediapipe.AudioDecoderOptions]: {
            audio_stream { stream_index: 0 }
          }
        })pb");
  CalculatorRunner runner(node_config);
  runner.MutableSidePackets()->Tag("INPUT_FILE_PATH") = MakePacket<std::string>(
      file::JoinPath(GetTestDataDir(kTestPackageRoot),
                     "sine_wave_1k_44100_stereo_2_sec_aac.audio"));
  MP_ASSERT_OK(runner.Run());
  MP_EXPECT_OK(runner.Outputs()
                   .Tag("AUDIO_HEADER")
                   .header.ValidateAsType<mediapipe::TimeSeriesHeader>());
  const mediapipe::TimeSeriesHeader& header =
      runner.Outputs()
          .Tag("AUDIO_HEADER")
          .header.Get<mediapipe::TimeSeriesHeader>();
  EXPECT_EQ(44100, header.sample_rate());
  EXPECT_EQ(2, header.num_channels());
  EXPECT_TRUE(runner.Outputs().Tag("AUDIO").packets.size() >=
              std::ceil(44100.0 * 2 / 1024));
}

}  // namespace
}  // namespace mediapipe
