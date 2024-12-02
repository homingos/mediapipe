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

# OpenGL utilities.

# TODO: move Compat.java in here, remove dep
# TODO: add tests
android_library(
    name = "glutil",
    srcs = glob(["**/*.java"]),
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/java/com/google/mediapipe/framework:android_framework_no_proguard",
        "@maven//:com_google_code_findbugs_jsr305",
        "@maven//:com_google_flogger_flogger",
        "@maven//:com_google_flogger_flogger_system_backend",
        "@maven//:com_google_guava_guava",
    ],
)

# Expose the java source files for building mediapipe AAR.
filegroup(
    name = "java_src",
    srcs = glob(["**/*.java"]),
    visibility = ["//mediapipe:__subpackages__"],
)
