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

package(default_visibility = ["//mediapipe/examples:__subpackages__"])

# Linux only
cc_binary(
    name = "hair_segmentation_gpu",
    data = ["//mediapipe/models:hair_segmentation.tflite"],
    deps = [
        "//mediapipe/examples/desktop:demo_run_graph_main_gpu",
        "//mediapipe/graphs/hair_segmentation:mobile_calculators",
    ],
)

cc_binary(
    name = "hair_segmentation_cpu",
    data = ["//mediapipe/models:hair_segmentation.tflite"],
    deps = [
        "//mediapipe/examples/desktop:demo_run_graph_main",
    ] + select({
        "//mediapipe/gpu:disable_gpu": [
            "//mediapipe/graphs/hair_segmentation:desktop_calculators",
        ],
        "//conditions:default": [
            "//mediapipe/graphs/hair_segmentation:mobile_calculators",
        ],
    }),
)
