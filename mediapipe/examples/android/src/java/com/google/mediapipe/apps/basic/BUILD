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

# Basic library common across example apps.
android_library(
    name = "basic_lib",
    srcs = glob(["*.java"]),
    manifest = "AndroidManifest.xml",
    resource_files = glob(["res/**"]),
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/java/com/google/mediapipe/components:android_camerax_helper",
        "//mediapipe/java/com/google/mediapipe/components:android_components",
        "//mediapipe/java/com/google/mediapipe/framework:android_framework",
        "//mediapipe/java/com/google/mediapipe/glutil",
        "//third_party:androidx_appcompat",
        "//third_party:androidx_constraint_layout",
        "//third_party:androidx_core",
        "//third_party:opencv",
        "//third_party:androidx_legacy_support_v4",
        "//third_party:camerax_camera2",
        "//third_party:camerax_core",
        "//third_party:camerax_lifecycle",
        "@maven//:androidx_concurrent_concurrent_futures",
        "@maven//:androidx_lifecycle_lifecycle_common",
        "@maven//:com_google_guava_guava",
        "@maven//:com_squareup_okhttp3_okhttp",
    ],
)

# Manifest common across example apps.
exports_files(
    srcs = ["AndroidManifest.xml"],
)

# Native dependencies to perform edge detection in the Hello World example.
cc_binary(
    name = "libmediapipe_jni.so",
    linkshared = 1,
    linkstatic = 1,
    deps = [
        # "//mediapipe/graphs/edge_detection:mobile_calculators",
        "//mediapipe/java/com/google/mediapipe/framework/jni:mediapipe_framework_jni",
    ],
)

# Converts the .so cc_binary into a cc_library, to be consumed in an android_binary.
cc_library(
    name = "mediapipe_jni_lib",
    srcs = [":libmediapipe_jni.so"],
    alwayslink = 1,
)

# Hello World example app.
android_binary(
    name = "helloworld",
    assets = [
        # "//mediapipe/graphs/edge_detection:mobile_gpu.binarypb",
    ],
    assets_dir = "",
    manifest = "AndroidManifest.xml",
    manifest_values = {
        "applicationId": "com.google.mediapipe.apps.basic",
        "appName": "Hello World",
        "mainActivity": ".MainActivity",
        "cameraFacingFront": "False",
        "binaryGraphName": "mobile_gpu.binarypb",
        "inputVideoStreamName": "input_video",
        "flipFramesVertically": "True",
        "converterNumBuffers": "2",
    },
    multidex = "native",
    deps = [
        ":basic_lib",
        ":mediapipe_jni_lib",
    ],
)
