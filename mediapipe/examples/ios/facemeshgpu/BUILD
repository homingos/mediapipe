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

load(
    "@build_bazel_rules_apple//apple:ios.bzl",
    "ios_application",
)
load(
    "//mediapipe/examples/ios:bundle_id.bzl",
    "BUNDLE_ID_PREFIX",
    "example_provisioning",
)

licenses(["notice"])

MIN_IOS_VERSION = "12.0"

alias(
    name = "facemeshgpu",
    actual = "FaceMeshGpuApp",
)

ios_application(
    name = "FaceMeshGpuApp",
    app_icons = ["//mediapipe/examples/ios/common:AppIcon"],
    bundle_id = BUNDLE_ID_PREFIX + ".FaceMeshGpu",
    families = [
        "iphone",
        "ipad",
    ],
    infoplists = [
        "//mediapipe/examples/ios/common:Info.plist",
        "Info.plist",
    ],
    minimum_os_version = MIN_IOS_VERSION,
    provisioning_profile = example_provisioning(),
    deps = [
        ":FaceMeshGpuAppLibrary",
        "@ios_opencv//:OpencvFramework",
    ],
)

objc_library(
    name = "FaceMeshGpuAppLibrary",
    srcs = [
        "FaceMeshGpuViewController.mm",
    ],
    hdrs = [
        "FaceMeshGpuViewController.h",
    ],
    copts = ["-std=c++17"],
    data = [
        "//mediapipe/graphs/face_mesh:face_mesh_mobile_gpu.binarypb",
        "//mediapipe/modules/face_detection:face_detection_short_range.tflite",
        "//mediapipe/modules/face_landmark:face_landmark_with_attention.tflite",
    ],
    deps = [
        "//mediapipe/examples/ios/common:CommonMediaPipeAppLibrary",
        "//mediapipe/framework/formats:landmark_cc_proto",
    ] + select({
        "//mediapipe:ios_i386": [],
        "//mediapipe:ios_x86_64": [],
        "//conditions:default": [
            "//mediapipe/graphs/face_mesh:mobile_calculators",
        ],
    }),
)
