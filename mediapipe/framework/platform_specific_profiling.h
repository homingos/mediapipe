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

#ifndef MEDIAPIPE_FRAMEWORK_PLATFORM_SPECIFIC_PROFILING_H_
#define MEDIAPIPE_FRAMEWORK_PLATFORM_SPECIFIC_PROFILING_H_

#ifdef MEDIAPIPE_PROFILER_AVAILABLE
#include "mediapipe/framework/profiler/graph_profiler.h"
#else
#include "mediapipe/framework/profiler/graph_profiler_stub.h"
#endif

// Enabling this flag, will require specific platform implementation for the
// methods mediapipe::PlatformSpecificTraceEventBegin() and
// mediapipe::PlatformSpecificTraceEventEnd().
#ifdef ENABLE_PLATFORM_SPECIFIC_PROFILING
namespace mediapipe {

const char kProfilingCategory[] = "Calculators";

void PlatformSpecificTraceEventBegin(const char* name, int64_t id,
                                     const char* category_group,
                                     int64_t packet_timestamp);
void PlatformSpecificTraceEventEnd(const char* name, int64_t id,
                                   const char* category_group,
                                   int64_t packet_timestamp);

// Temporary object to profile entry and exit of events.
class PlatformSpecificProfilingScope {
 public:
  PlatformSpecificProfilingScope(const char* name, int64_t id,
                                 int64_t packet_timestamp)
      : method_name_(TraceEvent::UNKNOWN),
        name_(name),
        id_(id),
        packet_timestamp_(packet_timestamp) {
    PlatformSpecificTraceEventBegin(name_, id_, kProfilingCategory,
                                    packet_timestamp_);
  }

  // Scope constructor that only starts profiling for Process method.
  // Useful to limit automated profiling to only Process().
  PlatformSpecificProfilingScope(const char* name, int64_t id,
                                 int64_t packet_timestamp,
                                 TraceEvent::EventType method_name)
      : method_name_(method_name),
        name_(name),
        id_(id),
        packet_timestamp_(packet_timestamp) {
    if (method_name_ == TraceEvent::PROCESS) {
      PlatformSpecificTraceEventBegin(name_, id_, kProfilingCategory,
                                      packet_timestamp_);
    }
  }

  ~PlatformSpecificProfilingScope() {
    if (method_name_ == TraceEvent::PROCESS ||
        method_name_ == TraceEvent::UNKNOWN) {
      PlatformSpecificTraceEventEnd(name_, id_, kProfilingCategory,
                                    packet_timestamp_);
    }
  }

 private:
  TraceEvent::EventType method_name_;
  const char* name_;
  int64_t id_;
  int64_t packet_timestamp_;
};
}  // namespace mediapipe

// General profiling macro.
#define PLATFORM_SPECIFIC_PROFILER(name, id, packet_timestamp)       \
  mediapipe::PlatformSpecificProfilingScope platform_specific_scope( \
      name, id, packet_timestamp);

// Automated profiling macro.
// Filters out all methods except Calculator::Process().
#define PLATFORM_SPECIFIC_PROCESS_PROFILER(name, id, method_name,    \
                                           packet_timestamp)         \
  mediapipe::PlatformSpecificProfilingScope platform_specific_scope( \
      name, id, packet_timestamp, mediapipe::TraceEvent::method_name);

#else
#define PLATFORM_SPECIFIC_PROFILER(name, id, packet_timestamp)
#define PLATFORM_SPECIFIC_PROCESS_PROFILER(name, id, method_name, \
                                           packet_timestamp)
#endif  // ENABLE_PLATFORM_SPECIFIC_PROFILING

#endif  // MEDIAPIPE_FRAMEWORK_PLATFORM_SPECIFIC_PROFILING_H_
