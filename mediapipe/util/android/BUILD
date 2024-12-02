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
    name = "asset_manager_util",
    srcs = select({
        "//conditions:default": [],  # don't build other than android.
        "//mediapipe:android": [
            "asset_manager_util.cc",
        ],
    }),
    hdrs = ["asset_manager_util.h"],
    defines = select({
        "//mediapipe/framework:android_no_jni": ["MEDIAPIPE_NO_JNI"],
        "//conditions:default": [],
    }),
    linkopts = select({
        "//conditions:default": [],
        "//mediapipe/framework:android_no_jni": [],
        "//mediapipe:android": ["-landroid"],
    }),
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework/port:ret_check",
        "//mediapipe/framework/port:singleton",
        "//mediapipe/framework/port:status",
        "//mediapipe/framework/port:statusor",
        "//mediapipe/util/android/file/base",
        "@com_google_absl//absl/log:absl_check",
        "@com_google_absl//absl/log:absl_log",
        "@com_google_absl//absl/strings",
    ] + select({
        "//conditions:default": [],
        "//mediapipe:android": [
            "//mediapipe/java/com/google/mediapipe/framework/jni:jni_util",
        ],
    }),
)

cc_library(
    name = "logging",
    hdrs = select({
        "//conditions:default": [],
        "//mediapipe:android": ["logging.h"],
    }),
    linkopts = [
        "-landroid",
        "-llog",
    ],
)
