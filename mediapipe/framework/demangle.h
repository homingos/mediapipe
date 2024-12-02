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

#ifndef MEDIAPIPE_FRAMEWORK_DEMANGLE_H_
#define MEDIAPIPE_FRAMEWORK_DEMANGLE_H_

#ifndef MEDIAPIPE_HAS_CXA_DEMANGLE
// We only support some compilers that support __cxa_demangle.
// TODO: Checks if Android NDK has fixed this issue or not.
#if defined(__ANDROID__) && (defined(__i386__) || defined(__x86_64__))
#define MEDIAPIPE_HAS_CXA_DEMANGLE 0
#elif (__GNUC__ >= 4 || (__GNUC__ >= 3 && __GNUC_MINOR__ >= 4)) && \
    !defined(__mips__)
#define MEDIAPIPE_HAS_CXA_DEMANGLE 1
#elif defined(__clang__) && !defined(_MSC_VER)
#define MEDIAPIPE_HAS_CXA_DEMANGLE 1
#else
#define MEDIAPIPE_HAS_CXA_DEMANGLE 0
#endif
#endif

#include <stdlib.h>

#include <string>
#if MEDIAPIPE_HAS_CXA_DEMANGLE
#include <cxxabi.h>
#endif

namespace mediapipe {

// Demangle a mangled symbol name and return the demangled name.
// If 'mangled' isn't mangled in the first place, this function
// simply returns 'mangled' as is.
//
// This function is used for demangling mangled symbol names such as
// '_Z3bazifdPv'.  It uses abi::__cxa_demangle() if your compiler has
// the API.  Otherwise, this function simply returns 'mangled' as is.
//
// Currently, we support only GCC 3.4.x or later for the following
// reasons.
//
// - GCC 2.95.3 doesn't have cxxabi.h
// - GCC 3.3.5 and ICC 9.0 have a bug.  Their abi::__cxa_demangle()
//   returns junk values for non-mangled symbol names (ex. function
//   names in C linkage).  For example,
//     abi::__cxa_demangle("main", 0,  0, &status)
//   returns "unsigned long" and the status code is 0 (successful).
//
// Also,
//
//  - MIPS is not supported because abi::__cxa_demangle() is not defined.
//  - Android x86 is not supported because STLs don't define __cxa_demangle
//
// Prefer using MediaPipeTypeStringOrDemangled<T>() when possible (defined
// in type_map.h).
inline std::string Demangle(const char* mangled) {
  int status = 0;
  char* demangled = nullptr;
#if MEDIAPIPE_HAS_CXA_DEMANGLE
  demangled = abi::__cxa_demangle(mangled, nullptr, nullptr, &status);
#endif
  std::string out;
  if (status == 0 && demangled != nullptr) {  // Demangling succeeeded.
    out.append(demangled);
    free(demangled);
  } else {
    out.append(mangled);
  }
  return out;
}

}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_DEMANGLE_H_
