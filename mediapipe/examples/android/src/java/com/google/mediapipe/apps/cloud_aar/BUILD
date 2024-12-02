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

load("//mediapipe/java/com/google/mediapipe:mediapipe_aar.bzl", "mediapipe_aar")

mediapipe_aar(
    name = "flam_scan_track",
    calculators = [
        "//mediapipe/graphs/custom:local_calculators",
    ],
    assets = [
        "//mediapipe/graphs/custom:feature_extraction_desktop.binarypb",
        "//mediapipe/graphs/custom:model.tflite"
    ],
    assets_dir = "",
)