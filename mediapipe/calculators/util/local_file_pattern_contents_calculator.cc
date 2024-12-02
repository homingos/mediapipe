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

#include <memory>
#include <string>

#include "absl/log/absl_log.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/file_helpers.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {

constexpr char kContentsTag[] = "CONTENTS";
constexpr char kFileSuffixTag[] = "FILE_SUFFIX";
constexpr char kFileDirectoryTag[] = "FILE_DIRECTORY";

// The calculator takes the path to local directory and desired file suffix to
// mach as input side packets, and outputs the contents of those files that
// match the pattern. Those matched files will be sent sequentially through the
// output stream with incremental timestamp difference by 1.
//
// Example config:
// node {
//   calculator: "LocalFilePatternContentsCalculator"
//   input_side_packet: "FILE_DIRECTORY:file_directory"
//   input_side_packet: "FILE_SUFFIX:file_suffix"
//   output_stream: "CONTENTS:contents"
// }
class LocalFilePatternContentsCalculator : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc) {
    cc->InputSidePackets().Tag(kFileDirectoryTag).Set<std::string>();
    cc->InputSidePackets().Tag(kFileSuffixTag).Set<std::string>();
    cc->Outputs().Tag(kContentsTag).Set<std::string>();
    return absl::OkStatus();
  }

  absl::Status Open(CalculatorContext* cc) override {
    MP_RETURN_IF_ERROR(mediapipe::file::MatchFileTypeInDirectory(
        cc->InputSidePackets().Tag(kFileDirectoryTag).Get<std::string>(),
        cc->InputSidePackets().Tag(kFileSuffixTag).Get<std::string>(),
        &filenames_));
    std::sort(filenames_.begin(), filenames_.end());
    return absl::OkStatus();
  }

  absl::Status Process(CalculatorContext* cc) override {
    if (current_output_ < filenames_.size()) {
      auto contents = absl::make_unique<std::string>();
      ABSL_LOG(INFO) << filenames_[current_output_];
      MP_RETURN_IF_ERROR(mediapipe::file::GetContents(
          filenames_[current_output_], contents.get()));
      ++current_output_;
      cc->Outputs()
          .Tag(kContentsTag)
          .Add(contents.release(), Timestamp(current_output_));
    } else {
      return tool::StatusStop();
    }
    return absl::OkStatus();
  }

 private:
  std::vector<std::string> filenames_;
  int current_output_ = 0;
};

REGISTER_CALCULATOR(LocalFilePatternContentsCalculator);

}  // namespace mediapipe
