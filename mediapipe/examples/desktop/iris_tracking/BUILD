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
    name = "iris_depth_from_image_desktop",
    srcs = ["iris_depth_from_image_desktop.cc"],
    data = ["//mediapipe/modules/iris_landmark:iris_landmark.tflite"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/formats:image_frame",
        "//mediapipe/framework/formats:image_frame_opencv",
        "//mediapipe/framework/port:file_helpers",
        "//mediapipe/framework/port:opencv_highgui",
        "//mediapipe/framework/port:opencv_imgcodecs",
        "//mediapipe/framework/port:opencv_imgproc",
        "//mediapipe/framework/port:opencv_video",
        "//mediapipe/framework/port:parse_text_proto",
        "//mediapipe/framework/port:status",
        "//mediapipe/graphs/iris_tracking:iris_depth_cpu_deps",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/log:absl_log",
    ],
)

cc_binary(
    name = "iris_tracking_cpu_video_input",
    data = ["//mediapipe/modules/iris_landmark:iris_landmark.tflite"],
    deps = [
        "//mediapipe/examples/desktop:simple_run_graph_main",
        "//mediapipe/graphs/iris_tracking:iris_tracking_cpu_video_input_deps",
    ],
)

cc_binary(
    name = "iris_tracking_cpu",
    data = ["//mediapipe/modules/iris_landmark:iris_landmark.tflite"],
    deps = [
        "//mediapipe/examples/desktop:demo_run_graph_main",
        "//mediapipe/graphs/iris_tracking:iris_tracking_cpu_deps",
    ],
)

# Linux only
cc_binary(
    name = "iris_tracking_gpu",
    data = ["//mediapipe/modules/iris_landmark:iris_landmark.tflite"],
    deps = [
        "//mediapipe/examples/desktop:demo_run_graph_main_gpu",
        "//mediapipe/graphs/iris_tracking:iris_tracking_gpu_deps",
    ],
)
