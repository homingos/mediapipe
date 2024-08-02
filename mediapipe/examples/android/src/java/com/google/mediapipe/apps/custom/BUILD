# Copyright 2019 The MediaPipe Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

licenses(["notice"])

package(default_visibility = ["//visibility:private"])


# load("//mediapipe/java/com/google/mediapipe:mediapipe_aar.bzl", "mediapipe_aar")

# mediapipe_aar(
#     name = "flam_scan_track",
#     calculators = [
#         "//mediapipe/graphs/custom:local_calculators"
#     ],
#     assets = [
#         "//mediapipe/graphs/custom:feature_extraction_desktop.binarypb",
#         "//mediapipe/graphs/custom:model.tflite",
#     ],
#     assets_dir = "",
    
# )


cc_binary(
    name = "libmediapipe_jni.so",
    linkshared = 1,
    linkstatic = 1,
    deps = [
        "//mediapipe/graphs/custom:local_calculators",
        "//mediapipe/java/com/google/mediapipe/framework/jni:mediapipe_framework_jni",
    ],
)

cc_library(
    name = "mediapipe_jni_lib",
    srcs = [":libmediapipe_jni.so"],
    alwayslink = 1,
)

android_binary(
    name = "custom",
    srcs = glob(["*.java"]),
    assets = [
        "//mediapipe/graphs/custom:feature_extraction_desktop.binarypb",
        "//mediapipe/graphs/custom:model.tflite"
    ],
    assets_dir = "",
    manifest = "//mediapipe/examples/android/src/java/com/google/mediapipe/apps/basic:AndroidManifest.xml",
    manifest_values = {
        "applicationId": "com.google.mediapipe.apps.custom",
        "appName": "RtCS",
        "mainActivity": "com.google.mediapipe.apps.basic.MainActivity",
        "cameraFacingFront": "False",
        "binaryGraphName": "feature_extraction_desktop.binarypb",
        "inputVideoStreamName": "input_video_gpu",
        "outputVideoStreamName": "output_video",
        "flipFramesVertically": "True",
        "converterNumBuffers": "8",
    },
    multidex = "native",
    deps = [
        ":mediapipe_jni_lib",
        "//mediapipe/examples/android/src/java/com/google/mediapipe/apps/basic:basic_lib",
        "//third_party:opencv",
    ],
)