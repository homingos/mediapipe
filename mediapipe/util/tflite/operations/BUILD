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

licenses(["notice"])

package(default_visibility = [
    "//research/aimatter/tflite:__subpackages__",
    "//mediapipe:__subpackages__",
    # For automated benchmarking of Camera models by TFLite team.
    "//learning/brain/models/app_benchmarks/camera_models:__subpackages__",
])

CUSTOM_OPS_PACKAGE = "@org_tensorflow//tensorflow/lite/delegates/gpu/common/mediapipe"

cc_library(
    name = "landmarks_to_transform_matrix",
    srcs = ["landmarks_to_transform_matrix.cc"],
    hdrs = ["landmarks_to_transform_matrix.h"],
    deps = [
        "@org_tensorflow//tensorflow/lite/delegates/gpu/common:types",
        "@org_tensorflow//tensorflow/lite/kernels:kernel_util",
        "@org_tensorflow//tensorflow/lite/kernels:padding",
        "@org_tensorflow//tensorflow/lite/kernels/internal:common",
        "@org_tensorflow//tensorflow/lite/kernels/internal:compatibility",
        "@org_tensorflow//tensorflow/lite/kernels/internal:tensor",
        "@org_tensorflow//tensorflow/lite/schema:schema_fbs",
        CUSTOM_OPS_PACKAGE + ":landmarks_to_transform_matrix",
    ],
)

cc_library(
    name = "max_pool_argmax",
    srcs = ["max_pool_argmax.cc"],
    hdrs = ["max_pool_argmax.h"],
    deps = [
        "@org_tensorflow//tensorflow/lite/kernels:kernel_util",
        "@org_tensorflow//tensorflow/lite/kernels:padding",
        "@org_tensorflow//tensorflow/lite/kernels/internal:common",
        "@org_tensorflow//tensorflow/lite/kernels/internal:tensor",
        "@org_tensorflow//tensorflow/lite/kernels/internal:tensor_utils",
    ],
)

cc_library(
    name = "max_unpooling",
    srcs = ["max_unpooling.cc"],
    hdrs = ["max_unpooling.h"],
    deps = [
        "@org_tensorflow//tensorflow/lite/kernels:kernel_util",
        "@org_tensorflow//tensorflow/lite/kernels:padding",
        "@org_tensorflow//tensorflow/lite/kernels/internal:common",
        "@org_tensorflow//tensorflow/lite/kernels/internal:tensor",
        "@org_tensorflow//tensorflow/lite/kernels/internal:tensor_utils",
    ],
)

cc_library(
    name = "transform_landmarks",
    srcs = ["transform_landmarks.cc"],
    hdrs = ["transform_landmarks.h"],
    deps = [
        "@org_tensorflow//tensorflow/lite/delegates/gpu/common:types",
        "@org_tensorflow//tensorflow/lite/kernels:kernel_util",
        "@org_tensorflow//tensorflow/lite/kernels:padding",
        "@org_tensorflow//tensorflow/lite/kernels/internal:common",
        "@org_tensorflow//tensorflow/lite/kernels/internal:compatibility",
        "@org_tensorflow//tensorflow/lite/kernels/internal:tensor",
        "@org_tensorflow//tensorflow/lite/schema:schema_fbs",
        CUSTOM_OPS_PACKAGE + ":transform_landmarks",
    ],
)

cc_library(
    name = "transform_tensor_bilinear",
    srcs = ["transform_tensor_bilinear.cc"],
    hdrs = ["transform_tensor_bilinear.h"],
    deps = [
        "@org_tensorflow//tensorflow/lite/delegates/gpu/common:types",
        "@org_tensorflow//tensorflow/lite/kernels:kernel_util",
        "@org_tensorflow//tensorflow/lite/kernels:padding",
        "@org_tensorflow//tensorflow/lite/kernels/internal:common",
        "@org_tensorflow//tensorflow/lite/kernels/internal:compatibility",
        "@org_tensorflow//tensorflow/lite/kernels/internal:tensor",
        "@org_tensorflow//tensorflow/lite/schema:schema_fbs",
        CUSTOM_OPS_PACKAGE + ":transform_tensor_bilinear",
    ],
)

cc_library(
    name = "transpose_conv_bias",
    srcs = ["transpose_conv_bias.cc"],
    hdrs = ["transpose_conv_bias.h"],
    deps = [
        "@org_tensorflow//tensorflow/lite/kernels:kernel_util",
        "@org_tensorflow//tensorflow/lite/kernels:padding",
        "@org_tensorflow//tensorflow/lite/kernels/internal:tensor",
        "@org_tensorflow//tensorflow/lite/kernels/internal:tensor_utils",
        "@org_tensorflow//tensorflow/lite/kernels/internal:types",
    ],
)

cc_library(
    name = "resampler",
    srcs = ["resampler.cc"],
    hdrs = ["resampler.h"],
    deps = [
        "@org_tensorflow//tensorflow/lite/c:c_api_types",
        "@org_tensorflow//tensorflow/lite/c:common",
        "@org_tensorflow//tensorflow/lite/kernels:kernel_util",
        "@org_tensorflow//tensorflow/lite/kernels/internal:tensor",
        "@org_tensorflow//tensorflow/lite/schema:schema_fbs",
    ],
)
