licenses(["notice"])

package(default_visibility = ["//visibility:public"])

load(
    "//mediapipe/framework/tool:mediapipe_graph.bzl",
    "mediapipe_binary_graph",
)

load(
    "//mediapipe/framework/tool:mediapipe_files.bzl",
    "mediapipe_files",
)

mediapipe_files(srcs = [
    "model.tflite",
])

cc_library(
    name = "local_calculators",
    deps = [
        "//mediapipe/calculators/core:flow_limiter_calculator",
        "//mediapipe/calculators/video:opencv_video_decoder_calculator",
        "//mediapipe/calculators/image:image_transformation_calculator",
        "//mediapipe/calculators/image:feature_detector_calculator_custom",
        "//mediapipe/calculators/image:image_localization_calculator",
        "//mediapipe/calculators/tflite:tflite_converter_calculator",
        "//mediapipe/calculators/tflite:tflite_inference_calculator",
        "//mediapipe/calculators/tflite:tflite_tensors_to_floats_calculator",
        "//mediapipe/calculators/tflite:tflite_tensors_to_bytestream_calculator",
    ],
)

mediapipe_binary_graph(
    name = "feature_extraction_desktop",
    graph = "feature_extraction_desktop.pbtxt",
    output_name = "feature_extraction_desktop.binarypb",
    deps = [
        ":local_calculators"
    ],
)