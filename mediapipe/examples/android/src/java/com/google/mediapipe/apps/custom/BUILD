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

# package(default_visibility = ["//visibility:private"])


load("//mediapipe/java/com/google/mediapipe:mediapipe_aar.bzl", "mediapipe_aar")

mediapipe_aar(
    name = "flam_scan_track",
    calculators = [
        "//mediapipe/graphs/custom:local_calculators",
    ],
    assets = [
        "//mediapipe/graphs/custom:feature_extraction_desktop.binarypb",
        "//mediapipe/graphs/custom:model.tflite"
    ],
    assets_dir = "",
)

# cc_binary(
#     name = "libmediapipe_jni.so",
#     linkshared = 1,
#     linkstatic = 1,
#     deps = [
#         "//mediapipe/graphs/custom:local_calculators",
#         "//mediapipe/java/com/google/mediapipe/framework/jni:mediapipe_framework_jni",
#     ],
# )

# cc_library(
#     name = "mediapipe_jni_lib",
#     srcs = [":libmediapipe_jni.so"],
#     alwayslink = 1,
# )

# android_binary(
#     name = "custom",
#     srcs = glob(["*.java"]),
#     assets = [
#         "//mediapipe/graphs/custom:feature_extraction_desktop.binarypb",
#         "//mediapipe/graphs/custom:model.tflite"
#     ],
#     assets_dir = "",
#     manifest = "//mediapipe/examples/android/src/java/com/google/mediapipe/apps/basic:AndroidManifest.xml",
#     manifest_values = {
#         "applicationId": "com.google.mediapipe.apps.custom",
#         "appName": "RtCS",
#         "mainActivity": "com.google.mediapipe.apps.basic.MainActivity",
#         "cameraFacingFront": "False",
#         "binaryGraphName": "feature_extraction_desktop.binarypb",
#         "inputVideoStreamName": "input_video_gpu",
#         "outputVideoStreamName": "output_video",
#         "flipFramesVertically": "True",
#         "converterNumBuffers": "20",
#     },
#     multidex = "native",
#     deps = [
#         ":mediapipe_jni_lib",
#         "//mediapipe/examples/android/src/java/com/google/mediapipe/apps/basic:basic_lib",
#     ],
# )