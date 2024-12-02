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

# Placeholder: load py_library
# Placeholder: load py_test

licenses(["notice"])

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "media_sequence_util",
    hdrs = ["media_sequence_util.h"],
    visibility = [
        "//home/interaction:__subpackages__",
        "//mediapipe:__subpackages__",
    ],
    deps = [
        "//mediapipe/framework/port:core_proto",
        "//mediapipe/framework/port:integral_types",
        "//mediapipe/framework/port:logging",
        "@com_google_absl//absl/log:absl_check",
        "@org_tensorflow//tensorflow/core:protos_all_cc",
    ],
)

cc_library(
    name = "media_sequence",
    srcs = ["media_sequence.cc"],
    hdrs = ["media_sequence.h"],
    visibility = [
        "//mediapipe:__subpackages__",
    ],
    deps = [
        ":media_sequence_util",
        "//mediapipe/framework/formats:location",
        "//mediapipe/framework/formats:matrix",
        "//mediapipe/framework/port:core_proto",
        "//mediapipe/framework/port:opencv_imgcodecs",
        "//mediapipe/framework/port:ret_check",
        "//mediapipe/framework/port:status",
        "@com_google_absl//absl/log:absl_check",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/strings",
        "@org_tensorflow//tensorflow/core:protos_all_cc",
    ],
)

cc_test(
    name = "media_sequence_util_test",
    srcs = ["media_sequence_util_test.cc"],
    deps = [
        ":media_sequence_util",
        "//mediapipe/framework/port:gtest_main",
        "//mediapipe/framework/port:parse_text_proto",
        "@org_tensorflow//tensorflow/core:protos_all_cc",
    ],
)

cc_test(
    name = "media_sequence_test",
    srcs = ["media_sequence_test.cc"],
    deps = [
        ":media_sequence",
        "//mediapipe/framework/formats:location",
        "//mediapipe/framework/port:gtest_main",
        "//mediapipe/framework/port:opencv_imgcodecs",
        "@org_tensorflow//tensorflow/core:protos_all_cc",
    ],
)
