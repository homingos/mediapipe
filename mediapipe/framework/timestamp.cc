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

#include "mediapipe/framework/timestamp.h"

#include <string>

#include "absl/log/absl_check.h"
#include "absl/log/absl_log.h"
#include "absl/strings/str_cat.h"

namespace mediapipe {

constexpr double Timestamp::kTimestampUnitsPerSecond;

// In the following functions:
// - The safe int type will check for overflow/underflow and other errors.
// - The CHECK in the constructor will disallow special values.
TimestampDiff Timestamp::operator-(const Timestamp other) const {
  ABSL_CHECK(IsRangeValue() && other.IsRangeValue())
      << "This timestamp is " << DebugString() << " and other was "
      << other.DebugString();
  TimestampBaseType tmp_base = timestamp_ - other.timestamp_;
  return TimestampDiff(tmp_base);
}
TimestampDiff TimestampDiff::operator+(const TimestampDiff other) const {
  TimestampBaseType tmp_base = timestamp_ + other.timestamp_;
  return TimestampDiff(tmp_base);
}
TimestampDiff TimestampDiff::operator-(const TimestampDiff other) const {
  TimestampBaseType tmp_base = timestamp_ - other.timestamp_;
  return TimestampDiff(tmp_base);
}

// Clamp the addition to the range [Timestamp::Min(), Timestamp::Max()].
Timestamp Timestamp::operator+(const TimestampDiff offset) const {
  ABSL_CHECK(IsRangeValue()) << "Timestamp is: " << DebugString();
  TimestampBaseType offset_base(offset.Value());
  if (offset_base >= TimestampBaseType(0)) {
    if (timestamp_.value() >= Timestamp::Max().Value() - offset_base.value()) {
      // We would overflow.
      return Timestamp::Max();
    }
  }
  if (offset_base <= TimestampBaseType(0)) {
    if (timestamp_.value() <= Timestamp::Min().Value() - offset_base.value()) {
      // We would underflow.
      return Timestamp::Min();
    }
  }
  return Timestamp(timestamp_ + offset_base);
}
Timestamp Timestamp::operator-(const TimestampDiff offset) const {
  return *this + -offset;
}
Timestamp TimestampDiff::operator+(const Timestamp timestamp) const {
  return timestamp + *this;
}

Timestamp Timestamp::operator+=(const TimestampDiff other) {
  *this = *this + other;
  return *this;
}
Timestamp Timestamp::operator-=(const TimestampDiff other) {
  *this = *this - other;
  return *this;
}
Timestamp Timestamp::operator++() {
  *this += 1;
  return *this;
}
Timestamp Timestamp::operator--() {
  *this -= 1;
  return *this;
}
Timestamp Timestamp::operator++(int /*unused*/) {
  Timestamp previous(*this);
  ++(*this);
  return previous;
}
Timestamp Timestamp::operator--(int /*unused*/) {
  Timestamp previous(*this);
  --(*this);
  return previous;
}

std::string Timestamp::DebugString() const {
  if (IsSpecialValue()) {
    if (*this == Timestamp::Unset()) {
      return "Timestamp::Unset()";
    } else if (*this == Timestamp::Unstarted()) {
      return "Timestamp::Unstarted()";
    } else if (*this == Timestamp::PreStream()) {
      return "Timestamp::PreStream()";
    } else if (*this == Timestamp::Min()) {
      return "Timestamp::Min()";
    } else if (*this == Timestamp::Max()) {
      return "Timestamp::Max()";
    } else if (*this == Timestamp::PostStream()) {
      return "Timestamp::PostStream()";
    } else if (*this == Timestamp::OneOverPostStream()) {
      return "Timestamp::OneOverPostStream()";
    } else if (*this == Timestamp::Done()) {
      return "Timestamp::Done()";
    } else {
      ABSL_LOG(FATAL) << "Unknown special type.";
    }
  }
  return absl::StrCat(timestamp_.value());
}
std::string TimestampDiff::DebugString() const {
  return absl::StrCat(timestamp_.value());
}

Timestamp Timestamp::NextAllowedInStream() const {
  if (*this >= Max() || *this == PreStream()) {
    // Indicates that no further timestamps may occur.
    return OneOverPostStream();
  } else if (*this < Min()) {
    return Min();
  }
  return *this + 1;
}

bool Timestamp::HasNextAllowedInStream() const {
  if (*this >= Max() || *this == PreStream()) {
    return false;
  }
  return true;
}

Timestamp Timestamp::PreviousAllowedInStream() const {
  if (*this <= Min() || *this == PostStream()) {
    // Indicates that no previous timestamps may occur.
    return Unstarted();
  } else if (*this > Max()) {
    return Max();
  }
  return *this - 1;
}

std::ostream& operator<<(std::ostream& os, Timestamp arg) {
  return os << arg.DebugString();
}
std::ostream& operator<<(std::ostream& os, TimestampDiff arg) {
  return os << arg.DebugString();
}

}  // namespace mediapipe
