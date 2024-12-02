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

#include <memory>
#include <string>

#include "absl/status/status.h"
#include "absl/strings/str_cat.h"
#include "absl/types/optional.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/calculator_runner.h"
#include "mediapipe/framework/port/gtest.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/port/status_matchers.h"

namespace mediapipe
{

  namespace
  {

    // A graph with using a BypassCalculator to pass through and ignore
    // most of its inputs and outputs.
    constexpr char kTestGraphConfig1[] = R"pb(
  type: "AppearancesPassThroughSubgraph"
  input_stream: "APPEARANCES:appearances"
  input_stream: "VIDEO:video_frame"
  input_stream: "FEATURE_CONFIG:feature_config"
  output_stream: "APPEARANCES:passthrough_appearances"
  output_stream: "FEDERATED_GAZE_OUTPUT:passthrough_federated_gaze_output"

  node {
    calculator: "BypassCalculator"
    input_stream: "PASS:appearances"
    input_stream: "IGNORE:0:video_frame"
    input_stream: "IGNORE:1:feature_config"
    output_stream: "PASS:passthrough_appearances"
    output_stream: "IGNORE:passthrough_federated_gaze_output"
    node_options: {
      [type.googleapis.com/mediapipe.BypassCalculatorOptions] {
        pass_input_stream: "PASS"
        pass_output_stream: "PASS"
      }
    }
  }
)pb";

    // A graph with using AppearancesPassThroughSubgraph as a do-nothing channel
    // for input frames and appearances.
    constexpr char kTestGraphConfig2[] = R"pb(
  input_stream: "VIDEO_FULL_RES:video_frame"
  input_stream: "APPEARANCES:input_appearances"
  input_stream: "FEATURE_CONFIG:feature_config"
  input_stream: "GAZE_ENABLED:gaze_enabled"
  output_stream: "APPEARANCES:analyzed_appearances"
  output_stream: "FEDERATED_GAZE_OUTPUT:federated_gaze_output"

  node {
    calculator: "SwitchContainer"
    input_stream: "VIDEO:video_frame"
    input_stream: "APPEARANCES:input_appearances"
    input_stream: "FEATURE_CONFIG:feature_config"
    input_stream: "ENABLE:gaze_enabled"
    output_stream: "APPEARANCES:analyzed_appearances"
    output_stream: "FEDERATED_GAZE_OUTPUT:federated_gaze_output"
    options {
      [mediapipe.SwitchContainerOptions.ext] {
        async_selection: true
        contained_node: { calculator: "AppearancesPassThroughSubgraph" }
      }
    }
  }
)pb";

    // A graph with using BypassCalculator as a do-nothing channel
    // for input frames and appearances.
    constexpr char kTestGraphConfig3[] = R"pb(
  input_stream: "VIDEO_FULL_RES:video_frame"
  input_stream: "APPEARANCES:input_appearances"
  input_stream: "FEATURE_CONFIG:feature_config"
  input_stream: "GAZE_ENABLED:gaze_enabled"
  output_stream: "APPEARANCES:analyzed_appearances"
  output_stream: "FEDERATED_GAZE_OUTPUT:federated_gaze_output"

  node {
    calculator: "SwitchContainer"
    input_stream: "VIDEO:video_frame"
    input_stream: "APPEARANCES:input_appearances"
    input_stream: "FEATURE_CONFIG:feature_config"
    input_stream: "ENABLE:gaze_enabled"
    output_stream: "APPEARANCES:analyzed_appearances"
    output_stream: "FEDERATED_GAZE_OUTPUT:federated_gaze_output"
    options {
      [mediapipe.SwitchContainerOptions.ext] {
        async_selection: true
        contained_node: {
          calculator: "BypassCalculator"
          node_options: {
            [type.googleapis.com/mediapipe.BypassCalculatorOptions] {
              pass_input_stream: "APPEARANCES"
              pass_output_stream: "APPEARANCES"
            }
          }
        }
      }
    }
  }
)pb";

    // A graph with using BypassCalculator as a disabled-gate
    // for input frames and appearances.
    constexpr char kTestGraphConfig4[] = R"pb(
  input_stream: "VIDEO_FULL_RES:video_frame"
  input_stream: "APPEARANCES:input_appearances"
  input_stream: "FEATURE_CONFIG:feature_config"
  input_stream: "GAZE_ENABLED:gaze_enabled"
  output_stream: "APPEARANCES:analyzed_appearances"
  output_stream: "FEDERATED_GAZE_OUTPUT:federated_gaze_output"

  node {
    calculator: "SwitchContainer"
    input_stream: "ENABLE:gaze_enabled"
    input_stream: "VIDEO:video_frame"
    input_stream: "APPEARANCES:input_appearances"
    input_stream: "FEATURE_CONFIG:feature_config"
    output_stream: "VIDEO:video_frame_out"
    output_stream: "APPEARANCES:analyzed_appearances"
    output_stream: "FEATURE_CONFIG:feature_config_out"
    options {
      [mediapipe.SwitchContainerOptions.ext] {
        contained_node: { calculator: "BypassCalculator" }
        contained_node: { calculator: "PassThroughCalculator" }
      }
    }
  }
)pb";

    // Reports packet timestamp and string contents, or "<empty>"".
    std::string DebugString(Packet p)
    {
      return absl::StrCat(p.Timestamp().DebugString(), ":",
                          p.IsEmpty() ? "<empty>" : p.Get<std::string>());
    }

    // Shows a bypass subgraph that passes through one stream.
    TEST(BypassCalculatorTest, SubgraphChannel)
    {
      CalculatorGraphConfig config_1 =
          mediapipe::ParseTextProtoOrDie<CalculatorGraphConfig>(kTestGraphConfig1);
      CalculatorGraphConfig config_2 =
          mediapipe::ParseTextProtoOrDie<CalculatorGraphConfig>(kTestGraphConfig2);
      CalculatorGraph graph;
      MP_ASSERT_OK(graph.Initialize({config_1, config_2}, {}));

      std::vector<std::string> analyzed_appearances;
      MP_ASSERT_OK(graph.ObserveOutputStream(
          "analyzed_appearances",
          [&](const Packet &p)
          {
            analyzed_appearances.push_back(DebugString(p));
            return absl::OkStatus();
          },
          true));
      std::vector<std::string> federated_gaze_output;
      MP_ASSERT_OK(graph.ObserveOutputStream(
          "federated_gaze_output",
          [&](const Packet &p)
          {
            federated_gaze_output.push_back(DebugString(p));
            return absl::OkStatus();
          },
          true));
      MP_ASSERT_OK(graph.StartRun({}));

      MP_ASSERT_OK(graph.AddPacketToInputStream(
          "input_appearances", MakePacket<std::string>("a1").At(Timestamp(200))));
      MP_ASSERT_OK(graph.AddPacketToInputStream(
          "video_frame", MakePacket<std::string>("v1").At(Timestamp(200))));
      MP_ASSERT_OK(graph.AddPacketToInputStream(
          "feature_config", MakePacket<std::string>("f1").At(Timestamp(200))));
      MP_ASSERT_OK(graph.WaitUntilIdle());

      EXPECT_THAT(analyzed_appearances, testing::ElementsAre("200:a1"));
      EXPECT_THAT(federated_gaze_output, testing::ElementsAre("200:<empty>"));

      MP_ASSERT_OK(graph.CloseAllInputStreams());
      MP_ASSERT_OK(graph.WaitUntilDone());
    }

    // Shows a BypassCalculator that passes through one stream.
    TEST(BypassCalculatorTest, CalculatorChannel)
    {
      CalculatorGraphConfig config_3 =
          mediapipe::ParseTextProtoOrDie<CalculatorGraphConfig>(kTestGraphConfig3);
      CalculatorGraph graph;
      MP_ASSERT_OK(graph.Initialize({config_3}, {}));

      std::vector<std::string> analyzed_appearances;
      MP_ASSERT_OK(graph.ObserveOutputStream(
          "analyzed_appearances",
          [&](const Packet &p)
          {
            analyzed_appearances.push_back(DebugString(p));
            return absl::OkStatus();
          },
          true));
      std::vector<std::string> federated_gaze_output;
      MP_ASSERT_OK(graph.ObserveOutputStream(
          "federated_gaze_output",
          [&](const Packet &p)
          {
            federated_gaze_output.push_back(DebugString(p));
            return absl::OkStatus();
          },
          true));
      MP_ASSERT_OK(graph.StartRun({}));

      MP_ASSERT_OK(graph.AddPacketToInputStream(
          "input_appearances", MakePacket<std::string>("a1").At(Timestamp(200))));
      MP_ASSERT_OK(graph.AddPacketToInputStream(
          "video_frame", MakePacket<std::string>("v1").At(Timestamp(200))));
      MP_ASSERT_OK(graph.AddPacketToInputStream(
          "feature_config", MakePacket<std::string>("f1").At(Timestamp(200))));
      MP_ASSERT_OK(graph.WaitUntilIdle());

      EXPECT_THAT(analyzed_appearances, testing::ElementsAre("200:a1"));
      EXPECT_THAT(federated_gaze_output, testing::ElementsAre("200:<empty>"));

      MP_ASSERT_OK(graph.CloseAllInputStreams());
      MP_ASSERT_OK(graph.WaitUntilDone());
    }

    // Shows a BypassCalculator that discards all inputs when ENABLED is false.
    TEST(BypassCalculatorTest, GatedChannel)
    {
      CalculatorGraphConfig config_3 =
          mediapipe::ParseTextProtoOrDie<CalculatorGraphConfig>(kTestGraphConfig4);
      CalculatorGraph graph;
      MP_ASSERT_OK(graph.Initialize({config_3}, {}));

      std::vector<std::string> analyzed_appearances;
      MP_ASSERT_OK(graph.ObserveOutputStream(
          "analyzed_appearances",
          [&](const Packet &p)
          {
            analyzed_appearances.push_back(DebugString(p));
            return absl::OkStatus();
          },
          true));
      std::vector<std::string> video_frame;
      MP_ASSERT_OK(graph.ObserveOutputStream(
          "video_frame_out",
          [&](const Packet &p)
          {
            video_frame.push_back(DebugString(p));
            return absl::OkStatus();
          },
          true));
      MP_ASSERT_OK(graph.StartRun({}));

      // Close the gate.
      MP_ASSERT_OK(graph.AddPacketToInputStream(
          "gaze_enabled", MakePacket<bool>(false).At(Timestamp(200))));
      MP_ASSERT_OK(graph.WaitUntilIdle());

      // Send packets at timestamp 200.
  MP_ASSERT_OK(graph.AddPacketToInputStream(
      "input_appearances", MakePacket<std::string>("a1")Timestamp(200))));
  MP_ASSERT_OK(graph.AddPacketToInputStream(
      "video_frame", MakePacket<std::string>("v1").At(Timestamp(200))));
  MP_ASSERT_OK(graph.AddPacketToInputStream(
      "feature_config", MakePacket<std::string>("f1").At(Timestamp(200))));
  MP_ASSERT_OK(graph.WaitUntilIdle());

  // Only timestamps arrive from the BypassCalculator.
  EXPECT_THAT(analyzed_appearances, testing::ElementsAre("200:<empty>"));
  EXPECT_THAT(video_frame, testing::ElementsAre("200:<empty>"));

  // Open the gate.
  MP_ASSERT_OK(graph.AddPacketToInputStream(
      "gaze_enabled", MakePacket<bool>(true).At(Timestamp(300))));
  MP_ASSERT_OK(graph.WaitUntilIdle());

  // Send packets at timestamp 300.
  MP_ASSERT_OK(graph.AddPacketToInputStream(
      "input_appearances", MakePacket<std::string>("a2").At(Timestamp(300))));
  MP_ASSERT_OK(graph.AddPacketToInputStream(
      "video_frame", MakePacket<std::string>("v2").At(Timestamp(300))));
  MP_ASSERT_OK(graph.AddPacketToInputStream(
      "feature_config", MakePacket<std::string>("f2").At(Timestamp(300))));
  MP_ASSERT_OK(graph.WaitUntilIdle());

  // Packets arrive from the PassThroughCalculator.
  EXPECT_THAT(analyzed_appearances,
              testing::ElementsAre("200:<empty>", "300:a2"));
  EXPECT_THAT(video_frame, testing::ElementsAre("200:<empty>", "300:v2"));

  MP_ASSERT_OK(graph.CloseAllInputStreams());
  MP_ASSERT_OK(graph.WaitUntilDone());
    }

  } // namespace

} // namespace mediapipe
