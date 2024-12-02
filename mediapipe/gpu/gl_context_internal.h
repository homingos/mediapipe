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

#ifndef MEDIAPIPE_GPU_GL_CONTEXT_INTERNAL_H_
#define MEDIAPIPE_GPU_GL_CONTEXT_INTERNAL_H_

#include "absl/synchronization/mutex.h"
#ifdef __APPLE__
#if TARGET_OS_OSX
#import <AppKit/NSOpenGL.h>
#else
#import <OpenGLES/EAGL.h>
#endif  // TARGET_OS_OSX
#endif  // __APPLE__

#include "mediapipe/gpu/gl_context.h"

namespace mediapipe {

class GlContext::DedicatedThread {
 public:
  DedicatedThread();
  ~DedicatedThread();
  DedicatedThread(const DedicatedThread&) = delete;
  DedicatedThread& operator=(DedicatedThread) = delete;

  absl::Status Run(GlStatusFunction gl_func);
  void RunWithoutWaiting(GlVoidFunction gl_func);

  bool IsCurrentThread();

  void SelfDestruct();

 private:
  static void* ThreadBody(void* instance);
  void ThreadBody();

  using Job = std::function<void(void)>;
  Job GetJob();
  void PutJob(Job job);

  absl::Mutex mutex_;
  // Used to wait for a job's completion.
  absl::CondVar gl_job_done_cv_ ABSL_GUARDED_BY(mutex_);
  pthread_t gl_thread_id_;

  std::deque<Job> jobs_ ABSL_GUARDED_BY(mutex_);
  absl::CondVar has_jobs_cv_ ABSL_GUARDED_BY(mutex_);

  bool self_destruct_ = false;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_GPU_GL_CONTEXT_INTERNAL_H_
