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

#include "absl/log/absl_log.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {

// This Calculator multiplexes several input streams into a single
// output stream, dropping input packets with timestamps older than the
// last output packet.  In case two packets arrive with the same timestamp,
// the packet with the lower stream index will be output and the rest will
// be dropped.
//
// This Calculator optionally produces a finish inidicator as its second
// output stream.  One indicator packet is produced for each input packet
// received.
//
// This Calculator can be used with an ImmediateInputStreamHandler or with the
// default ISH.
//
// This Calculator is designed to work with a Demux calculator such as
// the RoundRobinDemuxCalculator.  Therefore, packets from different
// input streams are normally not expected to have the same timestamp.
//
// NOTE: this calculator can drop packets non-deterministically, depending on
// how fast the input streams are fed. In most cases, MuxCalculator should be
// preferred. In particular, dropping packets can interfere with rate limiting
// mechanisms.
class ImmediateMuxCalculator : public CalculatorBase {
 public:
  // This calculator combines any set of input streams into a single
  // output stream.  All input stream types must match the output stream type.
  static absl::Status GetContract(CalculatorContract* cc);

  // Passes any input packet to the output stream immediately, unless the
  // packet timestamp is lower than a previously passed packet.
  absl::Status Process(CalculatorContext* cc) override;
  absl::Status Open(CalculatorContext* cc) override;
};
REGISTER_CALCULATOR(ImmediateMuxCalculator);

absl::Status ImmediateMuxCalculator::GetContract(CalculatorContract* cc) {
  RET_CHECK(cc->Outputs().NumEntries() >= 1 && cc->Outputs().NumEntries() <= 2)
      << "This calculator produces only one or two output streams.";
  cc->Outputs().Index(0).SetAny();
  if (cc->Outputs().NumEntries() >= 2) {
    cc->Outputs().Index(1).Set<bool>();
  }
  for (int i = 0; i < cc->Inputs().NumEntries(); ++i) {
    cc->Inputs().Index(i).SetSameAs(&cc->Outputs().Index(0));
  }
  return absl::OkStatus();
}

absl::Status ImmediateMuxCalculator::Open(CalculatorContext* cc) {
  cc->SetOffset(TimestampDiff(0));
  return absl::OkStatus();
}

absl::Status ImmediateMuxCalculator::Process(CalculatorContext* cc) {
  // Pass along the first packet, unless it has been superseded.
  for (int i = 0; i < cc->Inputs().NumEntries(); ++i) {
    const Packet& packet = cc->Inputs().Index(i).Value();
    if (!packet.IsEmpty()) {
      if (packet.Timestamp() >= cc->Outputs().Index(0).NextTimestampBound()) {
        cc->Outputs().Index(0).AddPacket(packet);
      } else {
        ABSL_LOG_FIRST_N(WARNING, 5)
            << "Dropping a packet with timestamp " << packet.Timestamp();
      }
      if (cc->Outputs().NumEntries() >= 2) {
        Timestamp output_timestamp = std::max(
            cc->InputTimestamp(), cc->Outputs().Index(1).NextTimestampBound());
        cc->Outputs().Index(1).Add(new bool(true), output_timestamp);
      }
    }
  }
  return absl::OkStatus();
}

}  // namespace mediapipe
