load(
    "//mediapipe/framework/tool:mediapipe_graph.bzl",
    "mediapipe_simple_subgraph",
)

licenses(["notice"])

package(default_visibility = ["//visibility:public"])

mediapipe_simple_subgraph(
    name = "tracking_subgraph",
    graph = "tracking_subgraph.pbtxt",
    register_as = "TrackingSubgraph",
    deps = [
        "//mediapipe/calculators/core:flow_limiter_calculator",
       "//mediapipe/calculators/core:previous_loopback_calculator",
       "//mediapipe/calculators/core:packet_cloner_calculator",
       "//mediapipe/calculators/image:overlay_calculator_custom",
       "//mediapipe/calculators/core:stream_to_side_packet_calculator",
       "//mediapipe/calculators/core:side_packet_to_stream_calculator",
    ],
)

mediapipe_simple_subgraph(
    name = "global_features_subgraph",
    graph = "global_features_subgraph.pbtxt",
    register_as = "GlobalFeaturesSubgraph",
    deps = [
        "//mediapipe/calculators/image:image_transformation_calculator",
        "//mediapipe/calculators/tflite:tflite_converter_calculator",
        "//mediapipe/calculators/tflite:tflite_inference_calculator",
        "//mediapipe/calculators/tflite:tflite_tensors_to_floats_calculator",
    ],
)