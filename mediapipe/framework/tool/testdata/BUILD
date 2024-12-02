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
#

load("//mediapipe/framework/port:build_config.bzl", "mediapipe_proto_library")
load(
    "//mediapipe/framework/tool:mediapipe_graph.bzl",
    "mediapipe_simple_subgraph",
)

licenses(["notice"])

package(default_visibility = [
    "//mediapipe:__subpackages__",
])

filegroup(
    name = "test_graph",
    srcs = ["test.pbtxt"],
)

exports_files([
    "test.pbtxt",
    "dub_quad_test_subgraph.pbtxt",
    "nested_test_subgraph.pbtxt",
    "single_flow_container_test.pbtxt",
    "dual_flow_container_test.pbtxt",
])

mediapipe_simple_subgraph(
    name = "dub_quad_test_subgraph",
    testonly = 1,
    graph = "dub_quad_test_subgraph.pbtxt",
    register_as = "DubQuadTestSubgraph",
    deps = [
        "//mediapipe/framework:test_calculators",
    ],
)

mediapipe_simple_subgraph(
    name = "nested_test_subgraph",
    testonly = 1,
    graph = "nested_test_subgraph.pbtxt",
    register_as = "NestedTestSubgraph",
    visibility = ["//mediapipe/framework:__subpackages__"],
    deps = [
        ":dub_quad_test_subgraph",
        "//mediapipe/framework:test_calculators",
    ],
)

mediapipe_proto_library(
    name = "frozen_generator_proto",
    srcs = ["frozen_generator.proto"],
    visibility = ["//mediapipe/framework:__subpackages__"],
    deps = [
        "//mediapipe/framework:packet_generator_proto",
    ],
)
