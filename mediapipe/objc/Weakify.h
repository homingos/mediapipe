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
// OBJC_LINTER

// TODO: check license. This was forked from an internal header.

/**
 * __WEAKNAME_ is a private macro used to generate a local variable name related
 * to the argument variable name. This generated local variable name is
 * intentionally stable across multiple invocations.
 */
#define __WEAKNAME_(variable) variable##_weak_

/**
 * WEAKIFY defines a new local variable that is a weak reference to the argument
 * variable.
 *
 * This macro is generally used to capture a weak reference to be captured by an
 * Objective-C block to avoid unintentionally extending an object's lifetime or
 * avoid causing a retain cycle.
 *
 * The new local variable's name will be based on the name of the target
 * variable and is stable across multiple invocations of WEAKIFY. In general,
 * you should not need to invoke WEAKIFY multiple times on the same variable.
 */
#define WEAKIFY(variable) \
  __weak __typeof__(variable) __WEAKNAME_(variable) = (variable)

/**
 * STRONGIFY defines a new shadow local variable with the same name as the
 * argument variable and initialize it with a resolved weak reference based on a
 * weak reference created previously using the WEAKIFY macro.
 *
 * @note:
 * This macro must be called within each block scope to prevent nested blocks
 * from capturing a strong reference from an outer block.
 */
#define STRONGIFY(variable)                            \
  _Pragma("clang diagnostic push")                     \
      _Pragma("clang diagnostic ignored \"-Wshadow\"") \
          __strong __typeof__(variable) variable =     \
              __WEAKNAME_(variable) _Pragma("clang diagnostic pop")
