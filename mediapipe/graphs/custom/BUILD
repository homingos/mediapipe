
load(
    "//mediapipe/framework/tool:mediapipe_graph.bzl",
    "mediapipe_binary_graph",
)

licenses(["notice"])
exports_files(["model.tflite"])
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "local_calculators",
    deps = [
        "//mediapipe/calculators/core:packet_sequencer_calculator",
        "//mediapipe/calculators/core:packet_cloner_calculator",
        "//mediapipe/calculators/core:flow_limiter_calculator",
        "//mediapipe/calculators/image:image_transformation_calculator",
        "//mediapipe/calculators/image:image_localization_calculator",
        "//mediapipe/calculators/image:feature_detector_calculator_custom",
        "//mediapipe/calculators/image:reranking_calculator",
        "//mediapipe/gpu:gpu_buffer_to_image_frame_calculator",
        "//mediapipe/gpu:image_frame_to_gpu_buffer_calculator",
        "//mediapipe/graphs/custom/subgraphs:tracking_subgraph",
        "//mediapipe/graphs/custom/subgraphs:global_features_subgraph",
        
    ],
)


mediapipe_binary_graph(
    name = "feature_extraction_desktop",
    graph = "feature_extraction_desktop.pbtxt",
    output_name = "feature_extraction_desktop.binarypb",
    deps = [
        ":local_calculators",
    ],
)
