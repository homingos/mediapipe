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

load("//mediapipe/framework/port:build_config.bzl", "mediapipe_proto_library")

licenses(["notice"])

package(default_visibility = ["//visibility:private"])

mediapipe_proto_library(
    name = "callback_packet_calculator_proto",
    srcs = ["callback_packet_calculator.proto"],
    visibility = ["//mediapipe/framework:__subpackages__"],
    deps = [
        "//mediapipe/framework:calculator_options_proto",
        "//mediapipe/framework:calculator_proto",
    ],
)

cc_library(
    name = "callback_packet_calculator",
    srcs = ["callback_packet_calculator.cc"],
    hdrs = ["callback_packet_calculator.h"],
    visibility = ["//mediapipe/framework:__subpackages__"],
    deps = [
        ":callback_packet_calculator_cc_proto",
        "//mediapipe/framework:calculator_base",
        "//mediapipe/framework:calculator_registry",
        "//mediapipe/framework:output_side_packet",
        "@com_google_absl//absl/status",
    ],
    alwayslink = 1,
)
