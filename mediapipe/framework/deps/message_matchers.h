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

#ifndef MEDIAPIPE_DEPS_MESSAGE_MATCHERS_H_
#define MEDIAPIPE_DEPS_MESSAGE_MATCHERS_H_

#include <memory>

#include "gmock/gmock.h"
#include "mediapipe/framework/port/core_proto_inc.h"

namespace mediapipe {

class ProtoMatcher {
 public:
  using is_gtest_matcher = void;
  using MessageType = proto_ns::MessageLite;

  explicit ProtoMatcher(const MessageType& message)
      : message_(CloneMessage(message)) {}

  bool MatchAndExplain(const MessageType& m,
                       testing::MatchResultListener*) const {
    return EqualsMessage(*message_, m);
  }
  bool MatchAndExplain(const MessageType* m,
                       testing::MatchResultListener*) const {
    return EqualsMessage(*message_, *m);
  }

  void DescribeTo(std::ostream* os) const {
    *os << "has the same serialization as " << ExpectedMessageDescription();
  }

  void DescribeNegationTo(std::ostream* os) const {
    *os << "does not have the same serialization as "
        << ExpectedMessageDescription();
  }

 private:
  std::unique_ptr<MessageType> CloneMessage(const MessageType& message) {
    std::unique_ptr<MessageType> clone(message.New());
    clone->CheckTypeAndMergeFrom(message);
    return clone;
  }

  bool EqualsMessage(const proto_ns::MessageLite& m_1,
                     const proto_ns::MessageLite& m_2) const {
    std::string s_1, s_2;
    m_1.SerializeToString(&s_1);
    m_2.SerializeToString(&s_2);
    return s_1 == s_2;
  }

  std::string ExpectedMessageDescription() const {
#if defined(MEDIAPIPE_PROTO_LITE)
    return "the expected message";
#else
    return message_->DebugString();
#endif
  }

  const std::shared_ptr<MessageType> message_;
};

inline ProtoMatcher EqualsProto(const proto_ns::MessageLite& message) {
  return ProtoMatcher(message);
}

// for Pointwise
MATCHER(EqualsProto, "") {
  const auto& a = ::testing::get<0>(arg);
  const auto& b = ::testing::get<1>(arg);
  return ::testing::ExplainMatchResult(EqualsProto(b), a, result_listener);
}

}  // namespace mediapipe

#endif  // MEDIAPIPE_DEPS_MESSAGE_MATCHERS_H_
