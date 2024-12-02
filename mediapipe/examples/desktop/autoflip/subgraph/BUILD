load("//mediapipe/framework/tool:mediapipe_graph.bzl", "mediapipe_simple_subgraph")

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

FACE_DETECTION_DEPS = [
    "//mediapipe/calculators/image:image_transformation_calculator",
    "//mediapipe/calculators/tflite:ssd_anchors_calculator",
    "//mediapipe/calculators/tflite:tflite_converter_calculator",
    "//mediapipe/calculators/tflite:tflite_inference_calculator",
    "//mediapipe/calculators/tflite:tflite_tensors_to_detections_calculator",
    "//mediapipe/calculators/util:detection_label_id_to_text_calculator",
    "//mediapipe/calculators/util:detection_letterbox_removal_calculator",
    "//mediapipe/calculators/util:non_max_suppression_calculator",
]

mediapipe_simple_subgraph(
    name = "autoflip_face_detection_subgraph",
    graph = "face_detection_subgraph.pbtxt",
    register_as = "AutoFlipFaceDetectionSubgraph",
    visibility = ["//visibility:public"],
    deps = FACE_DETECTION_DEPS,
)

mediapipe_simple_subgraph(
    name = "autoflip_front_face_detection_subgraph",
    graph = "front_face_detection_subgraph.pbtxt",
    register_as = "AutoFlipFrontFaceDetectionSubgraph",
    visibility = ["//visibility:public"],
    deps = FACE_DETECTION_DEPS,
)

mediapipe_simple_subgraph(
    name = "autoflip_object_detection_subgraph",
    graph = "autoflip_object_detection_subgraph.pbtxt",
    register_as = "AutoFlipObjectDetectionSubgraph",
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/graphs/object_detection:desktop_tflite_calculators",
    ],
)
