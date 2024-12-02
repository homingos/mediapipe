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

cc_binary(
    name = "face_mesh_tflite",
    data = ["//mediapipe/modules/face_landmark:face_landmark_with_attention.tflite"],
    deps = [
        "//mediapipe/examples/desktop:simple_run_graph_main",
        "//mediapipe/graphs/face_mesh:desktop_calculators",
    ],
)

cc_binary(
    name = "face_mesh_cpu",
    data = ["//mediapipe/modules/face_landmark:face_landmark_with_attention.tflite"],
    deps = [
        "//mediapipe/examples/desktop:demo_run_graph_main",
        "//mediapipe/graphs/face_mesh:desktop_live_calculators",
    ],
)

# Linux only
cc_binary(
    name = "face_mesh_gpu",
    data = ["//mediapipe/modules/face_landmark:face_landmark_with_attention.tflite"],
    deps = [
        "//mediapipe/examples/desktop:demo_run_graph_main_gpu",
        "//mediapipe/graphs/face_mesh:desktop_live_gpu_calculators",
    ],
)
