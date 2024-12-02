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
    "//mediapipe/framework/tool:mediapipe_files.bzl",
    "mediapipe_files",
)
load(
    "//mediapipe/framework/tool:mediapipe_graph.bzl",
    "mediapipe_simple_subgraph",
)

licenses(["notice"])

package(default_visibility = ["//visibility:public"])

mediapipe_simple_subgraph(
    name = "face_landmark_cpu",
    graph = "face_landmark_cpu.pbtxt",
    register_as = "FaceLandmarkCpu",
    deps = [
        ":face_landmarks_model_loader",
        ":tensors_to_face_landmarks",
        ":tensors_to_face_landmarks_with_attention",
        "//mediapipe/calculators/core:gate_calculator",
        "//mediapipe/calculators/core:split_vector_calculator",
        "//mediapipe/calculators/tensor:image_to_tensor_calculator",
        "//mediapipe/calculators/tensor:inference_calculator",
        "//mediapipe/calculators/tensor:tensors_to_floats_calculator",
        "//mediapipe/calculators/tensor:tensors_to_landmarks_calculator",
        "//mediapipe/calculators/tflite:tflite_custom_op_resolver_calculator",
        "//mediapipe/calculators/util:landmark_projection_calculator",
        "//mediapipe/calculators/util:thresholding_calculator",
        "//mediapipe/framework/tool:switch_container",
    ],
)

mediapipe_simple_subgraph(
    name = "face_landmark_gpu",
    graph = "face_landmark_gpu.pbtxt",
    register_as = "FaceLandmarkGpu",
    deps = [
        ":face_landmarks_model_loader",
        ":tensors_to_face_landmarks",
        ":tensors_to_face_landmarks_with_attention",
        "//mediapipe/calculators/core:gate_calculator",
        "//mediapipe/calculators/core:split_vector_calculator",
        "//mediapipe/calculators/tensor:image_to_tensor_calculator",
        "//mediapipe/calculators/tensor:inference_calculator",
        "//mediapipe/calculators/tensor:tensors_to_floats_calculator",
        "//mediapipe/calculators/tensor:tensors_to_landmarks_calculator",
        "//mediapipe/calculators/tflite:tflite_custom_op_resolver_calculator",
        "//mediapipe/calculators/util:landmark_projection_calculator",
        "//mediapipe/calculators/util:thresholding_calculator",
        "//mediapipe/framework/tool:switch_container",
    ],
)

mediapipe_simple_subgraph(
    name = "face_landmark_front_cpu",
    graph = "face_landmark_front_cpu.pbtxt",
    register_as = "FaceLandmarkFrontCpu",
    deps = [
        ":face_detection_front_detection_to_roi",
        ":face_landmark_cpu",
        ":face_landmark_landmarks_to_roi",
        "//mediapipe/calculators/core:begin_loop_calculator",
        "//mediapipe/calculators/core:clip_vector_size_calculator",
        "//mediapipe/calculators/core:constant_side_packet_calculator",
        "//mediapipe/calculators/core:end_loop_calculator",
        "//mediapipe/calculators/core:gate_calculator",
        "//mediapipe/calculators/core:previous_loopback_calculator",
        "//mediapipe/calculators/image:image_properties_calculator",
        "//mediapipe/calculators/util:association_norm_rect_calculator",
        "//mediapipe/calculators/util:collection_has_min_size_calculator",
        "//mediapipe/modules/face_detection:face_detection_short_range_cpu",
    ],
)

mediapipe_simple_subgraph(
    name = "face_landmark_front_gpu",
    graph = "face_landmark_front_gpu.pbtxt",
    register_as = "FaceLandmarkFrontGpu",
    deps = [
        ":face_detection_front_detection_to_roi",
        ":face_landmark_gpu",
        ":face_landmark_landmarks_to_roi",
        "//mediapipe/calculators/core:begin_loop_calculator",
        "//mediapipe/calculators/core:clip_vector_size_calculator",
        "//mediapipe/calculators/core:constant_side_packet_calculator",
        "//mediapipe/calculators/core:end_loop_calculator",
        "//mediapipe/calculators/core:gate_calculator",
        "//mediapipe/calculators/core:previous_loopback_calculator",
        "//mediapipe/calculators/image:image_properties_calculator",
        "//mediapipe/calculators/util:association_norm_rect_calculator",
        "//mediapipe/calculators/util:collection_has_min_size_calculator",
        "//mediapipe/modules/face_detection:face_detection_short_range_gpu",
    ],
)

mediapipe_simple_subgraph(
    name = "face_landmark_front_cpu_image",
    graph = "face_landmark_front_cpu_image.pbtxt",
    register_as = "FaceLandmarkFrontCpuImage",
    deps = [
        ":face_landmark_front_cpu",
        "//mediapipe/calculators/core:flow_limiter_calculator",
        "//mediapipe/calculators/image:image_transformation_calculator",
        "//mediapipe/calculators/util:from_image_calculator",
    ],
)

mediapipe_simple_subgraph(
    name = "face_landmark_front_gpu_image",
    graph = "face_landmark_front_gpu_image.pbtxt",
    register_as = "FaceLandmarkFrontGpuImage",
    deps = [
        ":face_landmark_front_gpu",
        "//mediapipe/calculators/core:flow_limiter_calculator",
        "//mediapipe/calculators/image:image_transformation_calculator",
        "//mediapipe/calculators/util:from_image_calculator",
    ],
)

mediapipe_files(
    srcs = [
        "face_landmark.tflite",
        "face_landmark_with_attention.tflite",
    ],
)

mediapipe_simple_subgraph(
    name = "face_detection_front_detection_to_roi",
    graph = "face_detection_front_detection_to_roi.pbtxt",
    register_as = "FaceDetectionFrontDetectionToRoi",
    deps = [
        "//mediapipe/calculators/util:detections_to_rects_calculator",
        "//mediapipe/calculators/util:rect_transformation_calculator",
    ],
)

mediapipe_simple_subgraph(
    name = "face_landmark_landmarks_to_roi",
    graph = "face_landmark_landmarks_to_roi.pbtxt",
    register_as = "FaceLandmarkLandmarksToRoi",
    deps = [
        "//mediapipe/calculators/util:detections_to_rects_calculator",
        "//mediapipe/calculators/util:landmarks_to_detection_calculator",
        "//mediapipe/calculators/util:rect_transformation_calculator",
    ],
)

mediapipe_simple_subgraph(
    name = "face_landmarks_model_loader",
    graph = "face_landmarks_model_loader.pbtxt",
    register_as = "FaceLandmarksModelLoader",
    deps = [
        "//mediapipe/calculators/core:constant_side_packet_calculator",
        "//mediapipe/calculators/tflite:tflite_model_calculator",
        "//mediapipe/calculators/util:local_file_contents_calculator",
        "//mediapipe/framework/tool:switch_container",
    ],
)

mediapipe_simple_subgraph(
    name = "tensors_to_face_landmarks",
    graph = "tensors_to_face_landmarks.pbtxt",
    register_as = "TensorsToFaceLandmarks",
    deps = [
        "//mediapipe/calculators/tensor:tensors_to_landmarks_calculator",
    ],
)

mediapipe_simple_subgraph(
    name = "tensors_to_face_landmarks_with_attention",
    graph = "tensors_to_face_landmarks_with_attention.pbtxt",
    register_as = "TensorsToFaceLandmarksWithAttention",
    deps = [
        "//mediapipe/calculators/core:split_vector_calculator",
        "//mediapipe/calculators/tensor:tensors_to_landmarks_calculator",
        "//mediapipe/calculators/util:landmarks_refinement_calculator",
    ],
)
