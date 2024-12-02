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

#include "mediapipe/util/header_util.h"

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/tool/status_util.h"

namespace mediapipe {

absl::Status CopyInputHeadersToOutputs(const InputStreamSet& inputs,
                                       const OutputStreamSet& outputs) {
  for (auto id = inputs.BeginId(); id < inputs.EndId(); ++id) {
    std::pair<std::string, int> tag_index = inputs.TagAndIndexFromId(id);
    auto output_id = outputs.GetId(tag_index.first, tag_index.second);
    if (output_id.IsValid()) {
      outputs.Get(output_id)->SetHeader(inputs.Get(id)->Header());
    }
  }

  return absl::OkStatus();
}

absl::Status CopyInputHeadersToOutputs(const InputStreamShardSet& inputs,
                                       OutputStreamShardSet* outputs) {
  for (auto id = inputs.BeginId(); id < inputs.EndId(); ++id) {
    std::pair<std::string, int> tag_index = inputs.TagAndIndexFromId(id);
    auto output_id = outputs->GetId(tag_index.first, tag_index.second);
    if (output_id.IsValid()) {
      outputs->Get(output_id).SetHeader(inputs.Get(id).Header());
    }
  }

  return absl::OkStatus();
}

}  // namespace mediapipe
