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

#include "absl/container/node_hash_map.h"
#include "mediapipe/calculators/util/timed_box_list_id_to_label_calculator.pb.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/packet.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/util/resource_util.h"
#include "mediapipe/util/tracking/box_tracker.pb.h"

#if defined(MEDIAPIPE_MOBILE)
#include "mediapipe/util/android/file/base/file.h"
#include "mediapipe/util/android/file/base/helpers.h"
#else
#include "mediapipe/framework/port/file_helpers.h"
#endif

namespace mediapipe {

using mediapipe::TimedBoxProto;
using mediapipe::TimedBoxProtoList;

// Takes a label map (from label IDs to names), and populate the label field in
// TimedBoxProto according to it's ID.
//
// Example usage:
// node {
//   calculator: "TimedBoxListIdToLabelCalculator"
//   input_stream: "input_timed_box_list"
//   output_stream: "output_timed_box_list"
//   node_options: {
//     [mediapipe.TimedBoxListIdToLabelCalculatorOptions] {
//       label_map_path: "labelmap.txt"
//     }
//   }
// }
class TimedBoxListIdToLabelCalculator : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc);

  absl::Status Open(CalculatorContext* cc) override;
  absl::Status Process(CalculatorContext* cc) override;

 private:
  absl::node_hash_map<int, std::string> label_map_;
};
REGISTER_CALCULATOR(TimedBoxListIdToLabelCalculator);

absl::Status TimedBoxListIdToLabelCalculator::GetContract(
    CalculatorContract* cc) {
  cc->Inputs().Index(0).Set<TimedBoxProtoList>();
  cc->Outputs().Index(0).Set<TimedBoxProtoList>();

  return absl::OkStatus();
}

absl::Status TimedBoxListIdToLabelCalculator::Open(CalculatorContext* cc) {
  cc->SetOffset(TimestampDiff(0));

  const auto& options =
      cc->Options<::mediapipe::TimedBoxListIdToLabelCalculatorOptions>();

  std::string string_path;
  MP_ASSIGN_OR_RETURN(string_path,
                      PathToResourceAsFile(options.label_map_path()));
  std::string label_map_string;
  MP_RETURN_IF_ERROR(file::GetContents(string_path, &label_map_string));

  std::istringstream stream(label_map_string);
  std::string line;
  int i = 0;
  while (std::getline(stream, line)) {
    label_map_[i++] = line;
  }
  return absl::OkStatus();
}

absl::Status TimedBoxListIdToLabelCalculator::Process(CalculatorContext* cc) {
  const auto& input_list = cc->Inputs().Index(0).Get<TimedBoxProtoList>();
  auto output_list = absl::make_unique<TimedBoxProtoList>();
  for (const auto& input_box : input_list.box()) {
    TimedBoxProto* box_ptr = output_list->add_box();
    *box_ptr = input_box;

    if (label_map_.find(input_box.id()) != label_map_.end()) {
      box_ptr->set_label(label_map_[input_box.id()]);
    }
  }
  cc->Outputs().Index(0).Add(output_list.release(), cc->InputTimestamp());
  return absl::OkStatus();
}

}  // namespace mediapipe
