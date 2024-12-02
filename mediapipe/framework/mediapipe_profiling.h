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

#ifndef MEDIAPIPE_FRAMEWORK_MEDIAPIPE_PROFILING_H_
#define MEDIAPIPE_FRAMEWORK_MEDIAPIPE_PROFILING_H_

#include "mediapipe/framework/platform_specific_profiling.h"
#ifdef MEDIAPIPE_PROFILER_AVAILABLE
#include "mediapipe/framework/profiler/graph_profiler.h"
#else
#include "mediapipe/framework/profiler/graph_profiler_stub.h"
#endif

#ifdef MEDIAPIPE_PROFILER_AVAILABLE
#define MEDIAPIPE_PROFILER_SCOPE_INTERNAL(event_type, calculator_context) \
  mediapipe::GraphProfiler::Scope graph_profiler_scope(                   \
      mediapipe::TraceEvent::event_type, calculator_context,              \
      calculator_context->GetProfilingContext())
#else
#define MEDIAPIPE_PROFILER_SCOPE_INTERNAL(method_name, calculator_context)
#endif

#define MEDIAPIPE_PROFILING(method_name, calculator_context) \
  MEDIAPIPE_PROFILER_SCOPE_INTERNAL(method_name, calculator_context)

namespace mediapipe {

// Log a TraceEvent to the GraphTracer.
inline void LogEvent(ProfilingContext* context, TraceEvent event) {
#ifdef MEDIAPIPE_PROFILER_AVAILABLE
  if (context) {
    context->LogEvent(event);
  }
#endif
}
}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_MEDIAPIPE_PROFILING_H_
