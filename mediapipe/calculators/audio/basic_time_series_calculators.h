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
//
// Abstract base class for basic MediaPipe calculators that operate on
// TimeSeries streams and don't require any Options protos.
// Subclasses must override ProcessMatrix, and optionally
// MutateHeader.

#ifndef MEDIAPIPE_CALCULATORS_AUDIO_BASIC_TIME_SERIES_CALCULATORS_H_
#define MEDIAPIPE_CALCULATORS_AUDIO_BASIC_TIME_SERIES_CALCULATORS_H_

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/matrix.h"
#include "mediapipe/framework/formats/time_series_header.pb.h"

namespace mediapipe {

class BasicTimeSeriesCalculatorBase : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc);
  absl::Status Open(CalculatorContext* cc) final;
  absl::Status Process(CalculatorContext* cc) final;

 protected:
  // Open() calls this method to mutate the output stream header.  The input
  // to this function will contain a copy of the input stream header, so
  // subclasses that do not need to mutate the header do not need to override
  // it.
  virtual absl::Status MutateHeader(TimeSeriesHeader* output_header);

  // Process() calls this method on each packet to compute the output matrix.
  virtual Matrix ProcessMatrix(const Matrix& input_matrix) = 0;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_CALCULATORS_AUDIO_BASIC_TIME_SERIES_CALCULATORS_H_
