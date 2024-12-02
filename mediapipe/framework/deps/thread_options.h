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

#ifndef MEDIAPIPE_DEPS_THREAD_OPTIONS_H_
#define MEDIAPIPE_DEPS_THREAD_OPTIONS_H_

#include <stddef.h>

#include <set>
#include <string>

namespace mediapipe {

// Options to configure a thread.  Default values are listed in
// the field descriptions.
class ThreadOptions {
 public:
  ThreadOptions() : stack_size_(0), nice_priority_level_(0) {}

  // Set the thread stack size (in bytes).  Passing stack_size==0 resets
  // the stack size to the default value for the system. The system default
  // is also the default for this class.
  ThreadOptions& set_stack_size(size_t stack_size) {
    stack_size_ = stack_size;
    return *this;
  }

  ThreadOptions& set_nice_priority_level(int nice_priority_level) {
    nice_priority_level_ = nice_priority_level;
    return *this;
  }

  ThreadOptions& set_cpu_set(const std::set<int>& cpu_set) {
    cpu_set_ = cpu_set;
    return *this;
  }

  ThreadOptions& set_name_prefix(const std::string& name_prefix) {
    name_prefix_ = name_prefix;
    return *this;
  }

  size_t stack_size() const { return stack_size_; }

  int nice_priority_level() const { return nice_priority_level_; }

  const std::set<int>& cpu_set() const { return cpu_set_; }

  std::string name_prefix() const { return name_prefix_; }

 private:
  size_t stack_size_;        // Size of thread stack
  int nice_priority_level_;  // Nice priority level of the workers
  std::set<int> cpu_set_;    // CPU set for affinity setting
  std::string name_prefix_;  // Name of the thread
};

}  // namespace mediapipe
#endif  // MEDIAPIPE_DEPS_THREAD_OPTIONS_H_
