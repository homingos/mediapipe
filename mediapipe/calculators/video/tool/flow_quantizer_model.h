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

// Quantization model to convert a real value float number (flow field) to a
// 8-bit discrete number.
#ifndef MEDIAPIPE_CALCULATORS_VIDEO_TOOL_FLOW_QUANTIZER_MODEL_H_
#define MEDIAPIPE_CALCULATORS_VIDEO_TOOL_FLOW_QUANTIZER_MODEL_H_

#include <cstdint>

#include "mediapipe/calculators/video/tool/flow_quantizer_model.pb.h"
#include "mediapipe/framework/formats/motion/optical_flow_field.h"
#include "mediapipe/framework/tool/status_util.h"

namespace mediapipe {

class FlowQuantizerModel {
 public:
  // Initializes the model proto.
  void Init();
  // Quantizes flow field with the model.
  uint8_t Apply(const float val, const int channel) const;
  // Loads model from proto.
  void LoadFromProto(const QuantizerModelData& data);
  // Gets proto from model.
  const QuantizerModelData& GetModelData() const;
  // Used in training. Updates the model proto by reading the flow fields.
  // TODO: This model is currently manually set. Need to find a way to
  // learn from flow fields directly.
  void AddSampleFlowField(const OpticalFlowField& flow);

 private:
  QuantizerModelData model_;
};
}  // namespace mediapipe

#endif  // MEDIAPIPE_CALCULATORS_VIDEO_TOOL_FLOW_QUANTIZER_MODEL_H_
