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

#ifndef MEDIAPIPE_FRAMEWORK_DEPS_RE2_H_
#define MEDIAPIPE_FRAMEWORK_DEPS_RE2_H_

#include <regex>  // NOLINT

namespace mediapipe {

// Implements a subset of RE2 using std::regex_match.
class RE2 {
 public:
  RE2(const std::string& pattern) : std_regex_(pattern) {}
  static bool FullMatch(std::string text, const RE2& re) {
    return std::regex_match(text, re.std_regex_);
  }
  static bool PartialMatch(std::string text, const RE2& re) {
    return std::regex_search(text, re.std_regex_);
  }
  static int GlobalReplace(std::string* text, const RE2& re,
                           const std::string& rewrite) {
    std::smatch sm;
    std::regex_search(*text, sm, re.std_regex_);
    *text = std::regex_replace(*text, re.std_regex_, rewrite);
    return std::max(0, static_cast<int>(sm.size()) - 1);
  }

 private:
  std::regex std_regex_;
};

// Implements LazyRE2 using std::call_once.
class LazyRE2 {
 public:
  RE2& operator*() const {
    std::call_once(once_, [&]() { ptr_ = new RE2(pattern_); });
    return *ptr_;
  }
  RE2* operator->() const { return &**this; }
  const char* pattern_;
  mutable RE2* ptr_;
  mutable std::once_flag once_;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_DEPS_RE2_H_
