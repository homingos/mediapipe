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
// MakeCleanup(f) returns an RAII cleanup object that calls 'f' in its
// destructor. The easiest way to use MakeCleanup is with a lambda argument,
// capturing the return value in an 'auto' local variable. Most users will not
// need more sophisticated syntax than that.
//
// Example:
//   void func() {}
//     FILE* fp = fopen("data.txt", "r");
//     if (fp == nullptr) return;
//     auto fp_cleaner = mediapipe::MakeCleanup([fp] { fclose(fp); });
//     // No matter what, fclose(fp) will happen.
//     DataObject d;
//     while (ReadDataObject(fp, &d)) {
//       if (d.IsBad()) {
//         ABSL_LOG(ERROR) << "Bad Data";
//         return;
//       }
//       PushGoodData(d);
//     }
//   }

#ifndef MEDIAPIPE_DEPS_CLEANUP_H_
#define MEDIAPIPE_DEPS_CLEANUP_H_

#include <type_traits>
#include <utility>

#include "absl/base/attributes.h"

namespace mediapipe {

template <typename F>
class Cleanup {
 public:
  Cleanup() : released_(true), f_() {}

  template <typename G>
  explicit Cleanup(G&& f)          // NOLINT
      : f_(std::forward<G>(f)) {}  // NOLINT(build/c++11)

  Cleanup(Cleanup&& src)  // NOLINT
      : released_(src.is_released()), f_(src.release()) {}

  // Implicitly move-constructible from any compatible Cleanup<G>.
  // The source will be released as if src.release() were called.
  // A moved-from Cleanup can be safely destroyed or reassigned.
  template <typename G>
  Cleanup(Cleanup<G>&& src)  // NOLINT
      : released_(src.is_released()), f_(src.release()) {}

  // Assignment to a Cleanup object behaves like destroying it
  // and making a new one in its place, analogous to unique_ptr
  // semantics.
  Cleanup& operator=(Cleanup&& src) {  // NOLINT
    if (!released_) f_();
    released_ = src.released_;
    f_ = src.release();
    return *this;
  }

  ~Cleanup() {
    if (!released_) f_();
  }

  // Releases the cleanup function instead of running it.
  // Hint: use c.release()() to run early.
  F release() {
    released_ = true;
    return std::move(f_);
  }

  bool is_released() const { return released_; }

 private:
  static_assert(!std::is_reference<F>::value, "F must not be a reference");

  bool released_ = false;
  F f_;
};

template <int&... ExplicitParameterBarrier, typename F,
          typename DecayF = typename std::decay<F>::type>
ABSL_MUST_USE_RESULT Cleanup<DecayF> MakeCleanup(F&& f) {
  static_assert(sizeof...(ExplicitParameterBarrier) == 0,
                "No explicit template arguments.");
  return Cleanup<DecayF>(std::forward<F>(f));
}

}  // namespace mediapipe

#endif  // MEDIAPIPE_DEPS_CLEANUP_H_
