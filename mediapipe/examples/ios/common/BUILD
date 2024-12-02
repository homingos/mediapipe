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

objc_library(
    name = "CommonMediaPipeAppLibrary",
    srcs = [
        "AppDelegate.mm",
        "CommonViewController.mm",
        "main.m",
    ],
    hdrs = [
        "AppDelegate.h",
        "CommonViewController.h",
    ],
    data = [
        "Base.lproj/LaunchScreen.storyboard",
        "Base.lproj/Main.storyboard",
    ],
    visibility = [
        "//mediapipe:__subpackages__",
    ],
    deps = [
        "//mediapipe/objc:mediapipe_framework_ios",
        "//mediapipe/objc:mediapipe_input_sources_ios",
        "//mediapipe/objc:mediapipe_layer_renderer",
        "//third_party/apple_frameworks:AVFoundation",
        "//third_party/apple_frameworks:CoreGraphics",
        "//third_party/apple_frameworks:CoreMedia",
        "//third_party/apple_frameworks:UIKit",
    ],
)

exports_files(["Info.plist"])

filegroup(
    name = "AppIcon",
    srcs = glob(["Assets.xcassets/AppIcon.appiconset/*"]),
    visibility = ["//mediapipe:__subpackages__"],
)
