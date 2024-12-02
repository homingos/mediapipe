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

android_library(
    name = "android_components",
    srcs = glob(
        ["*.java"],
        exclude = [
            "CameraHelper.java",
            "CameraXPreviewHelper.java",
        ],
    ),
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_java_proto_lite",
        "//mediapipe/java/com/google/mediapipe/framework:android_framework",
        "//mediapipe/java/com/google/mediapipe/glutil",
        "//third_party:androidx_appcompat",
        "//third_party:androidx_core",
        "//third_party:androidx_legacy_support_v4",
        "//third_party:androidx_recyclerview",
        "@maven//:com_google_code_findbugs_jsr305",
        "@maven//:com_google_guava_guava",
        "//mediapipe/java/com/google/mediapipe/components:android_camerax_helper",
        "//third_party:androidx_constraint_layout",
        "//third_party:opencv",
        "//third_party:camerax_camera2",
        "//third_party:camerax_core",
        "//third_party:camerax_lifecycle",
        "@maven//:androidx_concurrent_concurrent_futures",
        "@maven//:androidx_lifecycle_lifecycle_common",
        "@maven//:com_squareup_okhttp3_okhttp",
    ],
)

# Note: We need to separate the camera helper files in a different BUILD target because CameraX has a minimum Android API
# requirement of API 21. Users of android_components may have different API dependencies.
android_library(
    name = "android_camerax_helper",
    srcs = [
        "CameraHelper.java",
        "CameraXPreviewHelper.java",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/java/com/google/mediapipe/glutil",
        "//third_party:androidx_appcompat",
        "//third_party:androidx_core",
        "//third_party:androidx_legacy_support_v4",
        "//third_party:camerax_camera2",
        "//third_party:camerax_core",
        "//third_party:camerax_lifecycle",
        "@maven//:androidx_concurrent_concurrent_futures",
        "@maven//:androidx_lifecycle_lifecycle_common",
        "@maven//:com_google_code_findbugs_jsr305",
        "@maven//:com_google_guava_guava",
        "@maven//:com_google_guava_listenablefuture",
    ],
)

# Interfaces for common Audio Consumer and Producers in MediaPipe.
android_library(
    name = "android_audio_components",
    srcs = [
        "AudioDataConsumer.java",
        "AudioDataProcessor.java",
        "AudioDataProducer.java",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@maven//:com_google_code_findbugs_jsr305",
        "@maven//:com_google_guava_guava",
    ],
)

# MicrophoneHelper that provides access to audio data from a microphone
android_library(
    name = "android_microphone_helper",
    srcs = [
        "MicrophoneHelper.java",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":android_audio_components",
        "@maven//:com_google_guava_guava",
    ],
)

# Expose the java source files for building mediapipe AAR.
filegroup(
    name = "java_src",
    srcs = glob(["*.java"]),
    visibility = ["//mediapipe:__subpackages__"],
)
