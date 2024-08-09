
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
        
        "//mediapipe/gpu:gpu_buffer_to_image_frame_calculator",
        "//mediapipe/calculators/image:frame_joiner_calculator",
        "//mediapipe/calculators/image:image_localization_calculator",
        "//mediapipe/calculators/core:pass_through_calculator",
        "//mediapipe/calculators/core:gate_calculator",
        "//mediapipe/calculators/core:side_packet_to_stream_calculator",
        "//mediapipe/calculators/core:stream_to_side_packet_calculator",
        "//mediapipe/graphs/custom/subgraphs:tracking_subgraph",
        "//mediapipe/graphs/custom/subgraphs:scanning_subgraph",
        "//mediapipe/gpu:image_frame_to_gpu_buffer_calculator",
        "//mediapipe/calculators/core:packet_cloner_calculator",
         "//mediapipe/calculators/video:box_tracker_calculator",
        "//mediapipe/calculators/video:flow_packager_calculator",
        "//mediapipe/calculators/video:motion_analysis_calculator",
        "//mediapipe/framework/stream_handler:immediate_input_stream_handler",
        "//mediapipe/framework/stream_handler:sync_set_input_stream_handler",
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
