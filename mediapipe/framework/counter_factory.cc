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

#include "mediapipe/framework/counter_factory.h"

#include <vector>

#include "absl/log/absl_log.h"
#include "absl/strings/string_view.h"
#include "absl/synchronization/mutex.h"

namespace mediapipe {
namespace {

// Counter implementation when we're not using Flume.
// TODO: Consider using Dax atomic counters instead of this.
// This class is thread safe.
class BasicCounter : public Counter {
 public:
  explicit BasicCounter(const std::string& name) : value_(0) {}

  void Increment() ABSL_LOCKS_EXCLUDED(mu_) override {
    absl::WriterMutexLock lock(&mu_);
    ++value_;
  }

  void IncrementBy(int amount) ABSL_LOCKS_EXCLUDED(mu_) override {
    absl::WriterMutexLock lock(&mu_);
    value_ += amount;
  }

  int64_t Get() ABSL_LOCKS_EXCLUDED(mu_) override {
    absl::ReaderMutexLock lock(&mu_);
    return value_;
  }

 private:
  absl::Mutex mu_;
  int64_t value_ ABSL_GUARDED_BY(mu_);
};

}  // namespace

CounterSet::CounterSet() {}

CounterSet::~CounterSet() ABSL_LOCKS_EXCLUDED(mu_) { PublishCounters(); }

void CounterSet::PublishCounters() ABSL_LOCKS_EXCLUDED(mu_) {}

void CounterSet::PrintCounters() ABSL_LOCKS_EXCLUDED(mu_) {
  absl::ReaderMutexLock lock(&mu_);
  ABSL_LOG_IF(INFO, !counters_.empty()) << "MediaPipe Counters:";
  for (const auto& counter : counters_) {
    ABSL_LOG(INFO) << counter.first << ": " << counter.second->Get();
  }
}

Counter* CounterSet::Get(const std::string& name) ABSL_LOCKS_EXCLUDED(mu_) {
  absl::ReaderMutexLock lock(&mu_);
  if (!mediapipe::ContainsKey(counters_, name)) {
    return nullptr;
  }
  return counters_[name].get();
}

std::map<std::string, int64_t> CounterSet::GetCountersValues()
    ABSL_LOCKS_EXCLUDED(mu_) {
  absl::ReaderMutexLock lock(&mu_);
  std::map<std::string, int64_t> result;
  for (const auto& it : counters_) {
    result[it.first] = it.second->Get();
  }
  return result;
}

Counter* BasicCounterFactory::GetCounter(const std::string& name) {
  return counter_set_.Emplace<BasicCounter>(name, name);
}

}  // namespace mediapipe
