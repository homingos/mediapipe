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

#ifndef MEDIAPIPE_PORT_LOGGING_H_
#define MEDIAPIPE_PORT_LOGGING_H_

#include "absl/time/time.h"

#ifdef _WIN32
#define GLOG_NO_ABBREVIATED_SEVERITIES
#endif

#include <cstddef>
#include <ostream>
#include <type_traits>
#include <vector>

#include "absl/strings/has_ostream_operator.h"
#include "glog/logging.h"

namespace std {

template <typename... Ts, typename = std::enable_if_t<absl::HasOstreamOperator<
                              typename std::vector<Ts...>::value_type>::value>>
std::ostream& operator<<(std::ostream& out, const std::vector<Ts...>& v) {
  auto begin = v.begin();
  auto end = v.end();
  for (size_t i = 0; begin != end && i < 100; ++i, ++begin) {
    if (i) {
      out << " ";
    }
    out << *begin;
  }
  if (begin != end) {
    out << " ...";
  }
  return out;
}

}  // namespace std

namespace mediapipe {
using LogSeverity = google::LogSeverity;
const auto SetVLOGLevel = google::SetVLOGLevel;
class LogEntry {
 public:
  LogEntry(LogSeverity severity, const struct ::tm* tm_time,
           absl::string_view message)
      : severity_(severity),
        timestamp_(absl::FromTM(*tm_time, absl::LocalTimeZone())),
        text_message_(message) {}
  LogSeverity log_severity() const { return severity_; }
  absl::Time timestamp() const { return timestamp_; }
  absl::string_view text_message() const { return text_message_; }

 private:
  LogSeverity severity_;
  absl::Time timestamp_;
  absl::string_view text_message_;
};
class LogSink : public google::LogSink {
 public:
  virtual ~LogSink() = default;
  virtual void Send(const LogEntry& entry) = 0;
  virtual void WaitTillSent() {}

 private:
  virtual void send(LogSeverity severity, const char* full_filename,
                    const char* base_filename, int line,
                    const struct ::tm* tm_time, const char* message,
                    size_t message_len) {
    LogEntry log_entry(severity, tm_time,
                       absl::string_view(message, message_len));
    Send(log_entry);
  }
};
inline void AddLogSink(LogSink* destination) {
  google::AddLogSink(destination);
}
inline void RemoveLogSink(LogSink* destination) {
  google::RemoveLogSink(destination);
}
}  // namespace mediapipe

#endif  // MEDIAPIPE_PORT_LOGGING_H_
