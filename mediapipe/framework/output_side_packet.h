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

#ifndef MEDIAPIPE_FRAMEWORK_OUTPUT_SIDE_PACKET_H_
#define MEDIAPIPE_FRAMEWORK_OUTPUT_SIDE_PACKET_H_

#include "mediapipe/framework/packet.h"

namespace mediapipe {

// The OutputSidePacket base class defines the output side packet interface
// exposed to calculators in the CalculatorContext. The framework actually
// creates instances of the OutputSidePacketImpl subclass, which has other
// methods used by the framework.
class OutputSidePacket {
 public:
  OutputSidePacket() = default;
  virtual ~OutputSidePacket() = default;

  // Sets the output side packet. The Packet must contain the data.
  //
  // NOTE: Set() cannot report errors via the return value. It uses an error
  // callback function to report errors.
  virtual void Set(const Packet& packet) = 0;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_OUTPUT_SIDE_PACKET_H_
