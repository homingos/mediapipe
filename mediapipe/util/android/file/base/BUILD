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

licenses(["notice"])

cc_library(
    name = "base",
    srcs = [
        "file.cc",
        "filesystem.cc",
        "helpers.cc",
    ],
    hdrs = [
        "file.h",
        "filesystem.h",
        "helpers.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework/port:file_helpers",
        "//mediapipe/framework/port:status",
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/log:absl_log",
        "@com_google_absl//absl/strings",
    ],
)
