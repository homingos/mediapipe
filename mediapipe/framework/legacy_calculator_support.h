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

#ifndef MEDIAPIPE_FRAMEWORK_LEGACY_CALCULATOR_SUPPORT_H_
#define MEDIAPIPE_FRAMEWORK_LEGACY_CALCULATOR_SUPPORT_H_

#include "mediapipe/framework/calculator_context.h"
#include "mediapipe/framework/calculator_contract.h"

namespace mediapipe {

class LegacyCalculatorSupport {
 public:
  // Scoped is a RAII helper for setting the current CC in the current scope,
  // and unsetting it automatically (restoring the previous value) when
  // leaving the scope.
  //
  // This allows the current CC to be accessed at any point deeper in the
  // call stack of the current thread, until the scope is left. Creating
  // another Scoped instance deeper in the call stack applies to calls branching
  // from that point, and the previous value is restored when execution leaves
  // that scope, as one would expect.
  //
  // This is only meant to be used where backwards compatibility reasons prevent
  // passing the CC directly. Specifically, it can be used to access
  // CalculatorContext and CalculatorContract from legacy calculator code.
  template <class C>
  class Scoped {
   public:
    // The constructor saves the current value of current_ in an instance
    // member, which is then restored by the destructor.
    explicit Scoped(C* cc) {
      saved_ = current_;
      current_ = cc;
    }
    ~Scoped() { current_ = saved_; }

    // The current C* for this thread.
    static C* current() { return current_; }

   private:
    // The value to restore after exiting this scope.
    C* saved_;

    // This needs NOLINT because, when included in Objective-C++ files,
    // clang-tidy suggests using an Objective-C naming convention, which is
    // inappropriate. (b/116015736) No category specifier because of b/71698089.
    //
    // ABSL_CONST_INIT triggers b/155992786 with some versions of Clang on Apple
    // platforms.
#ifndef __APPLE__
    ABSL_CONST_INIT
#endif                                // !__APPLE__
    static thread_local C* current_;  // NOLINT
  };
};

#if !defined(_MSC_VER) || defined(__clang__)
// We only declare this variable for two specializations of the template because
// it is only meant to be used for these two types.
// Note that, since these variables are members of specific template
// _specializations_, they are not themselves templates, and therefore their
// definitions must be in the .cc file. However, a declaration still needs to be
// included in the header, or some compilers will assume they have no
// definition.
template <>
thread_local CalculatorContext*
    LegacyCalculatorSupport::Scoped<CalculatorContext>::current_;
template <>
thread_local CalculatorContract*
    LegacyCalculatorSupport::Scoped<CalculatorContract>::current_;
#endif

}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_LEGACY_CALCULATOR_SUPPORT_H_
