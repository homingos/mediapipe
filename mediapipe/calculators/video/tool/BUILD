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

load("//mediapipe/framework/port:build_config.bzl", "mediapipe_cc_proto_library")

licenses(["notice"])

package(default_visibility = ["//mediapipe/calculators/video:__subpackages__"])

proto_library(
    name = "flow_quantizer_model_proto",
    srcs = ["flow_quantizer_model.proto"],
    visibility = ["//visibility:public"],
)

mediapipe_cc_proto_library(
    name = "flow_quantizer_model_cc_proto",
    srcs = ["flow_quantizer_model.proto"],
    visibility = ["//visibility:public"],
    deps = [":flow_quantizer_model_proto"],
)

cc_library(
    name = "flow_quantizer_model",
    srcs = ["flow_quantizer_model.cc"],
    hdrs = ["flow_quantizer_model.h"],
    deps = [
        "//mediapipe/calculators/video/tool:flow_quantizer_model_cc_proto",
        "//mediapipe/framework:type_map",
        "//mediapipe/framework/formats:image_frame",
        "//mediapipe/framework/formats/motion:optical_flow_field",
        "//mediapipe/framework/port:integral_types",
        "//mediapipe/framework/port:ret_check",
        "//mediapipe/framework/tool:status_util",
        "@com_google_absl//absl/log:absl_check",
    ],
    alwayslink = 1,
)
