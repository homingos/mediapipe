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

#include "absl/strings/string_view.h"
#include "absl/strings/substitute.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/ret_check.h"

namespace mediapipe {

// Calculator that acts like the SQL query:
// SELECT *
// FROM packets_on_stream1 AS packet1
// INNER JOIN packets_on_stream2 AS packet2
// ON packet1.timestamp = packet2.timestamp
//
// In other words, it only emits and forwards packets if all input streams are
// not empty.
//
// Intended for use with FixedSizeInputStreamHandler.
//
// Related:
//   packet_cloner_calculator.cc: Repeats last-seen packets from empty inputs.
class PacketInnerJoinCalculator : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc);

  absl::Status Open(CalculatorContext* cc) override;
  absl::Status Process(CalculatorContext* cc) override;

 private:
  int num_streams_;
};

REGISTER_CALCULATOR(PacketInnerJoinCalculator);

absl::Status PacketInnerJoinCalculator::GetContract(CalculatorContract* cc) {
  RET_CHECK(cc->Inputs().NumEntries() == cc->Outputs().NumEntries())
      << "The number of input and output streams must match.";
  const int num_streams = cc->Inputs().NumEntries();
  for (int i = 0; i < num_streams; ++i) {
    cc->Inputs().Index(i).SetAny();
    cc->Outputs().Index(i).SetSameAs(&cc->Inputs().Index(i));
  }
  return absl::OkStatus();
}

absl::Status PacketInnerJoinCalculator::Open(CalculatorContext* cc) {
  num_streams_ = cc->Inputs().NumEntries();
  cc->SetOffset(TimestampDiff(0));
  return absl::OkStatus();
}

absl::Status PacketInnerJoinCalculator::Process(CalculatorContext* cc) {
  for (int i = 0; i < num_streams_; ++i) {
    if (cc->Inputs().Index(i).Value().IsEmpty()) {
      return absl::OkStatus();
    }
  }
  for (int i = 0; i < num_streams_; ++i) {
    cc->Outputs().Index(i).AddPacket(cc->Inputs().Index(i).Value());
  }
  return absl::OkStatus();
}

}  // namespace mediapipe
