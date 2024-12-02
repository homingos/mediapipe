// Copyright 2024 Flamingos Technologies Inc. All Rights Reserved.
//
// PROPRIETARY AND CONFIDENTIAL
//
// This software and its documentation are the confidential and proprietary
// information of Flamingos Technologies Inc. ("Confidential Information").
// You shall not disclose such Confidential Information and shall use it
// only in accordance with the terms of the license agreement you entered
// into with Flamingos Technologies Inc. Unauthorized copying, redistribution,
// or use of this software in source or binary forms is strictly prohibited.

#ifndef MEDIAPIPE_UTIL_TRACKING_MOTION_MODELS_CV_H_
#define MEDIAPIPE_UTIL_TRACKING_MOTION_MODELS_CV_H_

#include "mediapipe/framework/port/opencv_core_inc.h"
#include "mediapipe/util/tracking/motion_models.h"
#include "mediapipe/util/tracking/motion_models.pb.h"  // NOLINT

namespace mediapipe {

template <class Model>
class ModelCvConvert {};

// Specialized implementations, with additional functionality if needed.
template <>
class ModelCvConvert<TranslationModel> {
 public:
  // Returns 2x3 floating point cv::Mat with model parameters.
  static void ToCvMat(const TranslationModel& model, cv::Mat* matrix);
};

template <>
class ModelCvConvert<LinearSimilarityModel> {
 public:
  // Returns 2x3 floating point cv::Mat with model parameters.
  static void ToCvMat(const LinearSimilarityModel& model, cv::Mat* matrix);
};

template <>
class ModelCvConvert<AffineModel> {
 public:
  // Returns 2x3 floating point cv::Mat with model parameters.
  static void ToCvMat(const AffineModel& model, cv::Mat* matrix);
};

template <>
class ModelCvConvert<Homography> {
 public:
  // Returns 3x3 floating point cv::Mat with model parameters.
  static void ToCvMat(const Homography& model, cv::Mat* matrix);
};

template <class Model>
void ModelToCvMat(const Model& model, cv::Mat* matrix) {
  ModelCvConvert<Model>::ToCvMat(model, matrix);
}

}  // namespace mediapipe

#endif  // MEDIAPIPE_UTIL_TRACKING_MOTION_MODELS_CV_H_
