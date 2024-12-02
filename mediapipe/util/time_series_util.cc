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

#include "mediapipe/util/time_series_util.h"

#include <math.h>

#include <iostream>
#include <string>

#include "absl/log/absl_check.h"
#include "absl/log/absl_log.h"
#include "absl/strings/str_cat.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/time_series_header.pb.h"

namespace mediapipe {
namespace time_series_util {

bool LogWarningIfTimestampIsInconsistent(const Timestamp& current_timestamp,
                                         const Timestamp& initial_timestamp,
                                         int64_t cumulative_samples,
                                         double sample_rate) {
  // Ignore the "special" timestamp value Done().
  if (current_timestamp == Timestamp::Done()) return true;
  // Don't accept other special timestamp values.  We may need to change this
  // depending on how they're used in practice.
  if (!current_timestamp.IsRangeValue()) {
    ABSL_LOG(WARNING) << "Unexpected special timestamp: "
                      << current_timestamp.DebugString();
    return false;
  }

  // For non-special timestamp values, check whether the number of
  // samples that have been processed is consistent with amount of
  // time that has elapsed.
  double expected_timestamp_seconds =
      initial_timestamp.Seconds() + cumulative_samples / sample_rate;
  if (fabs(current_timestamp.Seconds() - expected_timestamp_seconds) >
      0.5 / sample_rate) {
    ABSL_LOG_EVERY_N(WARNING, 20)
        << std::fixed << "Timestamp " << current_timestamp.Seconds()
        << " not consistent with number of samples " << cumulative_samples
        << " and initial timestamp " << initial_timestamp
        << ".  Expected timestamp: " << expected_timestamp_seconds
        << " Timestamp difference: "
        << current_timestamp.Seconds() - expected_timestamp_seconds
        << " sample_rate: " << sample_rate;
    return false;
  } else {
    return true;
  }
}

absl::Status IsTimeSeriesHeaderValid(const TimeSeriesHeader& header) {
  if (header.has_sample_rate() && header.sample_rate() >= 0 &&
      header.has_num_channels() && header.num_channels() >= 0) {
    return absl::OkStatus();
  } else {
    std::string error_message =
        "TimeSeriesHeader is missing necessary fields: "
        "sample_rate or num_channels, or one of their values is negative. ";
#ifndef MEDIAPIPE_MOBILE
    absl::StrAppend(&error_message, "Got header:\n", header.ShortDebugString());
#endif
    return tool::StatusInvalid(error_message);
  }
}

absl::Status FillTimeSeriesHeaderIfValid(const Packet& header_packet,
                                         TimeSeriesHeader* header) {
  ABSL_CHECK(header);
  if (header_packet.IsEmpty()) {
    return tool::StatusFail("No header found.");
  }
  if (!header_packet.ValidateAsType<TimeSeriesHeader>().ok()) {
    return tool::StatusFail("Packet does not contain TimeSeriesHeader.");
  }
  *header = header_packet.Get<TimeSeriesHeader>();
  return IsTimeSeriesHeaderValid(*header);
}

absl::Status FillMultiStreamTimeSeriesHeaderIfValid(
    const Packet& header_packet, MultiStreamTimeSeriesHeader* header) {
  ABSL_CHECK(header);
  if (header_packet.IsEmpty()) {
    return tool::StatusFail("No header found.");
  }
  if (!header_packet.ValidateAsType<MultiStreamTimeSeriesHeader>().ok()) {
    return tool::StatusFail(
        "Packet does not contain MultiStreamTimeSeriesHeader.");
  }
  *header = header_packet.Get<MultiStreamTimeSeriesHeader>();
  if (!header->has_time_series_header()) {
    return tool::StatusFail("No time series header found.");
  }
  return IsTimeSeriesHeaderValid(header->time_series_header());
}

absl::Status IsMatrixShapeConsistentWithHeader(const Matrix& matrix,
                                               const TimeSeriesHeader& header) {
  if (header.has_num_samples() && matrix.cols() != header.num_samples()) {
    return tool::StatusInvalid(absl::StrCat(
        "Matrix size is inconsistent with header.  Expected ",
        header.num_samples(), " columns, but found ", matrix.cols()));
  }
  if (header.has_num_channels() && matrix.rows() != header.num_channels()) {
    return tool::StatusInvalid(absl::StrCat(
        "Matrix size is inconsistent with header.  Expected ",
        header.num_channels(), " rows, but found ", matrix.rows()));
  }
  return absl::OkStatus();
}

int64_t SecondsToSamples(double time_in_seconds, double sample_rate) {
  return round(time_in_seconds * sample_rate);
}

double SamplesToSeconds(int64_t num_samples, double sample_rate) {
  ABSL_DCHECK_NE(sample_rate, 0.0);
  return (num_samples / sample_rate);
}

}  // namespace time_series_util
}  // namespace mediapipe
