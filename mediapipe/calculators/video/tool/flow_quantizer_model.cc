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

#include "mediapipe/calculators/video/tool/flow_quantizer_model.h"

#include "absl/log/absl_check.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/type_map.h"

namespace mediapipe {

// Uniform normalization to 0-255.
uint8_t FlowQuantizerModel::Apply(const float val, const int channel) const {
  ABSL_CHECK_LT(channel, model_.min_value_size());
  const auto& min_value = model_.min_value(channel);
  const auto& max_value = model_.max_value(channel);
  QCHECK_GT(max_value, min_value);
  float res = (val - min_value) / (max_value - min_value);
  if (res < 0.0) {
    res = 0.0;
  } else if (res > 1.0) {
    res = 1.0;
  }
  return static_cast<uint8_t>(res * 255);
}

void FlowQuantizerModel::LoadFromProto(const QuantizerModelData& data) {
  QCHECK_GT(data.max_value(0), data.min_value(0));
  QCHECK_GT(data.max_value(1), data.min_value(1));

  model_ = data;
}

const QuantizerModelData& FlowQuantizerModel::GetModelData() const {
  return model_;
}

// Used for training, update the (min, max) range. We want to estimate the range
// of optical flow fields (Theorectically it is (-num_pixels_along_diag,
// num_pixels_along_diag).
// TODO: Taking the min and max over all training flow fields might be
// sensitive to noise. We should use more robust statistics.
void FlowQuantizerModel::AddSampleFlowField(const OpticalFlowField& flow) {
  ABSL_CHECK_EQ(model_.min_value_size(), 2);
  const cv::Mat_<cv::Point2f>& flow_mat = flow.flow_data();
  for (int i = 0; i != flow.width(); ++i) {
    for (int j = 0; j != flow.height(); ++j) {
      const auto& x = flow_mat.at<cv::Point2f>(i, j).x;
      const auto& y = flow_mat.at<cv::Point2f>(i, j).y;
      // Always use the minimum and maximum value occurred in training flow
      // fields.
      model_.set_min_value(0, std::min<float>(x, model_.min_value(0)));
      model_.set_min_value(1, std::min<float>(y, model_.min_value(1)));
      model_.set_max_value(0, std::max<float>(x, model_.max_value(0)));
      model_.set_max_value(1, std::max<float>(y, model_.max_value(1)));
    }
  }
}

void FlowQuantizerModel::Init() {
  model_.Clear();
  // Initialize the values.
  for (int i = 0; i != 2; ++i) {
    model_.add_min_value(std::numeric_limits<float>::max());
    model_.add_max_value(-std::numeric_limits<float>::max());
  }
}
}  // namespace mediapipe
