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

licenses(["notice"])

package(
    default_visibility = ["//visibility:public"],
    features = ["-layering_check"],
)

mediapipe_proto_library(
    name = "default_input_stream_handler_proto",
    srcs = ["default_input_stream_handler.proto"],
    deps = ["//mediapipe/framework:mediapipe_options_proto"],
    alwayslink = 1,
)

mediapipe_proto_library(
    name = "fixed_size_input_stream_handler_proto",
    srcs = ["fixed_size_input_stream_handler.proto"],
    deps = ["//mediapipe/framework:mediapipe_options_proto"],
    alwayslink = 1,
)

mediapipe_proto_library(
    name = "sync_set_input_stream_handler_proto",
    srcs = ["sync_set_input_stream_handler.proto"],
    deps = ["//mediapipe/framework:mediapipe_options_proto"],
    alwayslink = 1,
)

mediapipe_proto_library(
    name = "timestamp_align_input_stream_handler_proto",
    srcs = ["timestamp_align_input_stream_handler.proto"],
    deps = ["//mediapipe/framework:mediapipe_options_proto"],
    alwayslink = 1,
)

cc_library(
    name = "barrier_input_stream_handler",
    srcs = ["barrier_input_stream_handler.cc"],
    hdrs = ["barrier_input_stream_handler.h"],
    deps = [
        "//mediapipe/framework:calculator_context_manager",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework:collection_item_id",
        "//mediapipe/framework:input_stream_handler",
        "//mediapipe/framework:mediapipe_options_cc_proto",
        "//mediapipe/framework/tool:tag_map",
        "@com_google_absl//absl/log:absl_check",
        "@com_google_absl//absl/status",
    ],
    alwayslink = 1,
)

cc_library(
    name = "default_input_stream_handler",
    srcs = ["default_input_stream_handler.cc"],
    hdrs = ["default_input_stream_handler.h"],
    deps = [
        ":default_input_stream_handler_cc_proto",
        "//mediapipe/framework:input_stream_handler",
        "@com_google_absl//absl/strings",
    ],
    alwayslink = 1,
)

cc_library(
    name = "early_close_input_stream_handler",
    srcs = ["early_close_input_stream_handler.cc"],
    hdrs = ["early_close_input_stream_handler.h"],
    deps = [
        "//mediapipe/framework:calculator_context_manager",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework:collection_item_id",
        "//mediapipe/framework:input_stream_handler",
        "//mediapipe/framework:mediapipe_options_cc_proto",
        "//mediapipe/framework/tool:tag_map",
        "@com_google_absl//absl/log:absl_check",
        "@com_google_absl//absl/strings",
    ],
    alwayslink = 1,
)

cc_library(
    name = "fixed_size_input_stream_handler",
    srcs = ["fixed_size_input_stream_handler.cc"],
    hdrs = ["fixed_size_input_stream_handler.h"],
    deps = [
        ":default_input_stream_handler",
        ":fixed_size_input_stream_handler_cc_proto",
        "//mediapipe/framework:calculator_context_manager",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework:collection_item_id",
        "//mediapipe/framework:input_stream_handler",
        "//mediapipe/framework:mediapipe_options_cc_proto",
        "//mediapipe/framework:packet",
        "//mediapipe/framework/tool:tag_map",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/log:absl_check",
        "@com_google_absl//absl/log:absl_log",
        "@com_google_absl//absl/synchronization",
    ],
    alwayslink = 1,
)

cc_library(
    name = "immediate_input_stream_handler",
    srcs = ["immediate_input_stream_handler.cc"],
    hdrs = ["immediate_input_stream_handler.h"],
    deps = [
        "//mediapipe/framework:calculator_context_manager",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework:collection_item_id",
        "//mediapipe/framework:input_stream_handler",
        "//mediapipe/framework:mediapipe_options_cc_proto",
        "//mediapipe/framework/tool:tag_map",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/log:absl_check",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/synchronization",
    ],
    alwayslink = 1,
)

cc_library(
    name = "in_order_output_stream_handler",
    srcs = ["in_order_output_stream_handler.cc"],
    hdrs = ["in_order_output_stream_handler.h"],
    deps = [
        "//mediapipe/framework:collection",
        "//mediapipe/framework:collection_item_id",
        "//mediapipe/framework:mediapipe_options_cc_proto",
        "//mediapipe/framework:output_stream_handler",
        "//mediapipe/framework:output_stream_manager",
        "//mediapipe/framework:output_stream_shard",
        "//mediapipe/framework:packet_set",
        "//mediapipe/framework:timestamp",
        "//mediapipe/framework/tool:tag_map",
        "@com_google_absl//absl/log:absl_check",
    ],
    alwayslink = 1,
)

cc_library(
    name = "mux_input_stream_handler",
    srcs = ["mux_input_stream_handler.cc"],
    hdrs = ["mux_input_stream_handler.h"],
    deps = [
        "//mediapipe/framework:calculator_context_manager",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework:collection_item_id",
        "//mediapipe/framework:input_stream_handler",
        "@com_google_absl//absl/log:absl_check",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/synchronization",
    ],
    alwayslink = 1,
)

cc_library(
    name = "sync_set_input_stream_handler",
    srcs = ["sync_set_input_stream_handler.cc"],
    hdrs = ["sync_set_input_stream_handler.h"],
    deps = [
        ":sync_set_input_stream_handler_cc_proto",
        "//mediapipe/framework:calculator_context_manager",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework:collection_item_id",
        "//mediapipe/framework:input_stream_handler",
        "//mediapipe/framework:mediapipe_options_cc_proto",
        "//mediapipe/framework:packet_set",
        "//mediapipe/framework:timestamp",
        "//mediapipe/framework/port:map_util",
        "//mediapipe/framework/port:status",
        "//mediapipe/framework/tool:tag_map",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/log:absl_check",
        "@com_google_absl//absl/log:absl_log",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/synchronization",
    ],
    alwayslink = 1,
)

cc_library(
    name = "timestamp_align_input_stream_handler",
    srcs = ["timestamp_align_input_stream_handler.cc"],
    hdrs = ["timestamp_align_input_stream_handler.h"],
    deps = [
        ":timestamp_align_input_stream_handler_cc_proto",
        "//mediapipe/framework:calculator_context_manager",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework:collection_item_id",
        "//mediapipe/framework:input_stream_handler",
        "//mediapipe/framework:mediapipe_options_cc_proto",
        "//mediapipe/framework:timestamp",
        "//mediapipe/framework/tool:validate_name",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/log:absl_check",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/synchronization",
    ],
    alwayslink = 1,
)

cc_test(
    name = "barrier_input_stream_handler_test",
    srcs = ["barrier_input_stream_handler_test.cc"],
    deps = [
        ":barrier_input_stream_handler",
        "//mediapipe/framework:calculator_context",
        "//mediapipe/framework:calculator_context_manager",
        "//mediapipe/framework:input_stream_handler",
        "//mediapipe/framework/port:gtest_main",
        "//mediapipe/framework/tool:tag_map",
        "//mediapipe/framework/tool:tag_map_helper",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/log:absl_check",
        "@com_google_absl//absl/memory",
    ],
)

cc_test(
    name = "immediate_input_stream_handler_test",
    srcs = ["immediate_input_stream_handler_test.cc"],
    deps = [
        ":immediate_input_stream_handler",
        "//mediapipe/framework:calculator_context",
        "//mediapipe/framework:calculator_context_manager",
        "//mediapipe/framework:input_stream_handler",
        "//mediapipe/framework:packet",
        "//mediapipe/framework:timestamp",
        "//mediapipe/framework/port:gtest_main",
        "//mediapipe/framework/tool:tag_map",
        "//mediapipe/framework/tool:tag_map_helper",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/log:absl_check",
        "@com_google_absl//absl/memory",
    ],
)

cc_test(
    name = "mux_input_stream_handler_test",
    srcs = ["mux_input_stream_handler_test.cc"],
    deps = [
        ":mux_input_stream_handler",
        "//mediapipe/calculators/core:gate_calculator",
        "//mediapipe/calculators/core:make_pair_calculator",
        "//mediapipe/calculators/core:mux_calculator",
        "//mediapipe/calculators/core:pass_through_calculator",
        "//mediapipe/calculators/core:round_robin_demux_calculator",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/port:gtest_main",
        "//mediapipe/framework/port:parse_text_proto",
    ],
)

cc_test(
    name = "set_input_stream_handler_test",
    srcs = ["set_input_stream_handler_test.cc"],
    deps = [
        ":fixed_size_input_stream_handler",
        ":fixed_size_input_stream_handler_cc_proto",
        ":mux_input_stream_handler",
        "//mediapipe/calculators/core:mux_calculator",
        "//mediapipe/calculators/core:pass_through_calculator",
        "//mediapipe/calculators/core:round_robin_demux_calculator",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/port:gtest_main",
        "//mediapipe/framework/port:parse_text_proto",
        "//mediapipe/framework/port:ret_check",
    ],
)

cc_test(
    name = "default_input_stream_handler_test",
    srcs = ["default_input_stream_handler_test.cc"],
    deps = [
        ":default_input_stream_handler",
        "//mediapipe/calculators/core:pass_through_calculator",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/port:gtest_main",
        "//mediapipe/framework/port:parse_text_proto",
    ],
)

cc_test(
    name = "fixed_size_input_stream_handler_test",
    srcs = ["fixed_size_input_stream_handler_test.cc"],
    deps = [
        ":fixed_size_input_stream_handler",
        ":fixed_size_input_stream_handler_cc_proto",
        "//mediapipe/calculators/core:counting_source_calculator",
        "//mediapipe/calculators/core:pass_through_calculator",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/port:gtest_main",
        "//mediapipe/framework/port:logging",
        "//mediapipe/framework/port:parse_text_proto",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/synchronization",
    ],
)

cc_test(
    name = "sync_set_input_stream_handler_test",
    srcs = ["sync_set_input_stream_handler_test.cc"],
    deps = [
        ":sync_set_input_stream_handler",
        ":sync_set_input_stream_handler_cc_proto",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework:test_calculators",
        "//mediapipe/framework/port:gtest_main",
        "//mediapipe/framework/port:parse_text_proto",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/synchronization",
    ],
)

cc_test(
    name = "timestamp_align_input_stream_handler_test",
    srcs = ["timestamp_align_input_stream_handler_test.cc"],
    deps = [
        ":timestamp_align_input_stream_handler",
        "//mediapipe/calculators/core:packet_cloner_calculator",
        "//mediapipe/calculators/core:pass_through_calculator",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/port:gtest_main",
        "//mediapipe/framework/port:parse_text_proto",
    ],
)
