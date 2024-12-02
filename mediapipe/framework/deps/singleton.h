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

#ifndef MEDIAPIPE_DEPS_SINGLETON_H_
#define MEDIAPIPE_DEPS_SINGLETON_H_

#include "absl/synchronization/mutex.h"

// The Singleton template class creates a single instance of template parameter
// |T| when needed in a thread-safe fashion. A pointer to this single instance
// may be retrieved through a call to get().
template <typename T>
class Singleton {
 public:
  // Returns the pointer to the singleton of type |T|.
  // This method is thread-safe.
  static T *get() ABSL_LOCKS_EXCLUDED(mu_) {
    absl::MutexLock lock(&mu_);
    if (instance_) {
      return instance_;
    }

    if (destroyed_) {
      return nullptr;
    }
    if (instance_) {
      return instance_;
    }
    instance_ = new T();
    return instance_;
  }

  // Destroys the singleton . This method is only partially thread-safe.
  // It ensures that instance_ gets destroyed only once, and once destroyed, it
  // cannot be recreated. However, the callers of this method responsible for
  // making sure that no other threads are accessing (or plan to access) the
  // singleton any longer.
  static void Destruct() ABSL_LOCKS_EXCLUDED(mu_) {
    absl::MutexLock lock(&mu_);
    T *tmp_ptr = instance_;
    instance_ = nullptr;
    delete tmp_ptr;
    destroyed_ = true;
  }

 private:
  static T *instance_ ABSL_GUARDED_BY(mu_);
  static bool destroyed_ ABSL_GUARDED_BY(mu_);
  static absl::Mutex mu_;
};

template <typename T>
T *Singleton<T>::instance_ = nullptr;

template <typename T>
bool Singleton<T>::destroyed_ = false;

template <typename T>
absl::Mutex Singleton<T>::mu_;

#endif  // MEDIAPIPE_DEPS_SINGLETON_H_
