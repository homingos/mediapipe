#
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
#
# Description:
#   Working with dense optical flow in mediapipe.

load("//mediapipe/framework/port:build_config.bzl", "mediapipe_proto_library")

licenses(["notice"])

package(default_visibility = ["//visibility:public"])

mediapipe_proto_library(
    name = "optical_flow_field_data_proto",
    srcs = ["optical_flow_field_data.proto"],
)

cc_library(
    name = "optical_flow_field",
    srcs = ["optical_flow_field.cc"],
    hdrs = ["optical_flow_field.h"],
    deps = [
        ":optical_flow_field_data_cc_proto",
        "//mediapipe/framework:type_map",
        "//mediapipe/framework/deps:mathutil",
        "//mediapipe/framework/formats:location",
        "//mediapipe/framework/formats:location_opencv",
        "//mediapipe/framework/port:file_helpers",
        "//mediapipe/framework/port:integral_types",
        "//mediapipe/framework/port:opencv_imgproc",
        "//mediapipe/framework/port:point",
        "//mediapipe/framework/port:ret_check",
        "//mediapipe/framework/port:status",
        "@com_google_absl//absl/log:absl_check",
        "@com_google_absl//absl/log:absl_log",
        "@com_google_absl//absl/strings",
        "@org_tensorflow//tensorflow/core:framework",
    ],
    alwayslink = 1,
)

cc_test(
    name = "optical_flow_field_test",
    srcs = ["optical_flow_field_test.cc"],
    linkstatic = 1,
    deps = [
        ":optical_flow_field",
        "//mediapipe/framework/deps:file_path",
        "//mediapipe/framework/formats:location_opencv",
        "//mediapipe/framework/port:file_helpers",
        "//mediapipe/framework/port:gtest_main",
        "//mediapipe/framework/port:integral_types",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/log:absl_check",
        "@com_google_absl//absl/log:absl_log",
        "@org_tensorflow//tensorflow/core:framework",
    ],
)
