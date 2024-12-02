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

#ifndef MEDIAPIPE_FRAMEWORK_OUTPUT_STREAM_POLLER_H_
#define MEDIAPIPE_FRAMEWORK_OUTPUT_STREAM_POLLER_H_

#include <memory>

#include "absl/log/absl_check.h"
#include "mediapipe/framework/graph_output_stream.h"

namespace mediapipe {

// The public interface of output stream poller.
class OutputStreamPoller {
 public:
  OutputStreamPoller(const OutputStreamPoller&) = delete;
  OutputStreamPoller& operator=(const OutputStreamPoller&) = delete;
  OutputStreamPoller(OutputStreamPoller&&) = default;
  // Move assignment needs to be explicitly defaulted to allow
  // MP_ASSIGN_OR_RETURN on `StatusOr<OutputStreamPoller>`.
  OutputStreamPoller& operator=(OutputStreamPoller&&) = default;

  // Resets OutputStramPollerImpl and cleans the internal packet queue.
  void Reset() {
    auto poller = internal_poller_impl_.lock();
    ABSL_CHECK(poller) << "OutputStreamPollerImpl is already destroyed.";
    poller->Reset();
  }

  // Gets the next packet (block until it is available or the stream is
  // done).  Returns true if successful.
  ABSL_MUST_USE_RESULT bool Next(Packet* packet) {
    auto poller = internal_poller_impl_.lock();
    if (!poller) {
      return false;
    }
    return poller->Next(packet);
  }

  void SetMaxQueueSize(int queue_size) {
    auto poller = internal_poller_impl_.lock();
    ABSL_CHECK(poller) << "OutputStreamPollerImpl is already destroyed.";
    return poller->SetMaxQueueSize(queue_size);
  }

  // Returns the number of packets in the queue.
  int QueueSize() {
    auto poller = internal_poller_impl_.lock();
    ABSL_CHECK(poller) << "OutputStreamPollerImpl is already destroyed.";
    return poller->QueueSize();
  }

 private:
  OutputStreamPoller(
      std::shared_ptr<internal::OutputStreamPollerImpl> internal_poller_impl)
      : internal_poller_impl_(internal_poller_impl) {}

  std::weak_ptr<internal::OutputStreamPollerImpl> internal_poller_impl_;

  // Friend class to connect OutputStreamPoller with
  // internal::OutputStreamPollerImpl.
  friend class CalculatorGraph;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_OUTPUT_STREAM_POLLER_H_
