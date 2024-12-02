# Copyright 2024 Flamingos Technologies Inc. All Rights Reserved.
#
# PROPRIETARY AND CONFIDENTIAL
# 
# This software and its documentation are the confidential and proprietary
# information of Flamingos Technologies Inc. ("Confidential Information").
# You shall not disclose such Confidential Information and shall use it
# only in accordance with the terms of the license agreement you entered
# into with Flamingos Technologies Inc. Unauthorized copying, redistribution,
# or use of this software in source or binary forms is strictly prohibited.

# Placeholder: load py_binary
# Placeholder: load py_library

licenses(["notice"])

package(default_visibility = ["//mediapipe/examples:__subpackages__"])

cc_library(
    name = "run_graph_file_io_main",
    srcs = ["run_graph_file_io_main.cc"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/port:file_helpers",
        "//mediapipe/framework/port:map_util",
        "//mediapipe/framework/port:parse_text_proto",
        "//mediapipe/framework/port:status",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/log:absl_log",
        "@com_google_absl//absl/strings",
    ],
)

cc_binary(
    name = "media_sequence_demo",
    deps = [
        ":run_graph_file_io_main",
        "//mediapipe/graphs/media_sequence:clipped_images_from_file_at_24fps_calculators",
        "//mediapipe/graphs/media_sequence:tvl1_flow_and_rgb_from_file_calculators",
    ],
)
