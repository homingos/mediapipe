licenses(["notice"])

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "custom",
    linkstatic = True,
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/formats:image_frame",
        "//mediapipe/framework/port:opencv_imgcodecs",
        "//mediapipe/framework/port:status",
        "//mediapipe/framework/port:parse_text_proto",
        "//mediapipe/graphs/custom:local_calculators",
        "//mediapipe/examples/desktop:simple_run_graph_main"
    ],
)