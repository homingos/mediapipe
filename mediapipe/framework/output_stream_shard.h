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

#ifndef MEDIAPIPE_FRAMEWORK_OUTPUT_STREAM_SHARD_H_
#define MEDIAPIPE_FRAMEWORK_OUTPUT_STREAM_SHARD_H_

#include <list>
#include <string>

#include "absl/log/absl_check.h"
#include "mediapipe/framework/output_stream.h"
#include "mediapipe/framework/packet.h"
#include "mediapipe/framework/packet_type.h"
#include "mediapipe/framework/port.h"
#include "mediapipe/framework/timestamp.h"

namespace mediapipe {

class OutputStreamManager;

// The output stream spec shared across all output stream shards and their
// output stream manager.
struct OutputStreamSpec {
  // Triggers the error callback with absl::Status info when an error
  // occurs.
  void TriggerErrorCallback(const absl::Status& status) const {
    ABSL_CHECK(error_callback);
    error_callback(status);
  }

  std::string name;
  const PacketType* packet_type;
  std::function<void(absl::Status)> error_callback;
  bool locked_intro_data;
  // Those three variables are the intro data protected by locked_intro_data.
  bool offset_enabled;
  TimestampDiff offset;
  Packet header;
};

// OutputStreamShard, a subclass of OutputStream, holds an output queue
// and a timetstamp bound of an output stream. Each call to
// Calculator::Open(), Calculator::Process(), and Calculator::Close() can only
// access its own OutputStreamShard.
class OutputStreamShard : public OutputStream {
 public:
  OutputStreamShard();

  void SetSpec(OutputStreamSpec* output_stream_spec);

  // TODO Remove this interface from OutputStream? No client is using
  // this API.
  const std::string& Name() const final;

  // Sets the next timestamp bound in the OutputStreamShard.
  void SetNextTimestampBound(Timestamp timestamp) final;
  // Returns the next timestamp bound.
  Timestamp NextTimestampBound() const final { return next_timestamp_bound_; }

  // Marks the stream as closed in the OutputStreamShard. However, the output
  // stream will still be open until the OutputStreamHandler processes the
  // OutputStreamShard and executes OutputStreamManager::Close().
  void Close() final;
  bool IsClosed() const final;

  // Sets the offset.
  void SetOffset(TimestampDiff offset) final;

  TimestampDiff Offset() const final { return output_stream_spec_->offset; }

  bool OffsetEnabled() const final {
    return output_stream_spec_->offset_enabled;
  }

  // Sets the stream header.
  void SetHeader(const Packet& packet) final;
  // Returns a const reference to the header packet.
  const Packet& Header() const final;

  // Adds a packet to the output stream shard.
  void AddPacket(const Packet& packet) final;
  // Takes an rvalue reference of the packet and moves the packet to the output
  // stream shard.
  void AddPacket(Packet&& packet) final;

  // Returns true if the output queue is empty.
  bool IsEmpty() const { return output_queue_.empty(); }
  // Returns the timestamp of the last added packet in the output queue.
  Timestamp LastAddedPacketTimestamp() const;

 private:
  // AddPacketInternal template is called by either AddPacket(Packet&& packet)
  // or AddPacket(const Packet& packet).
  template <typename T>
  absl::Status AddPacketInternal(T&& packet);

  // Returns a pointer to the output queue.
  std::list<Packet>* OutputQueue() { return &output_queue_; }
  const std::list<Packet>* OutputQueue() const { return &output_queue_; }

  // Resets data members.
  void Reset(Timestamp next_timestamp_bound, bool close);

  // A pointer to the output stream spec object, which is owned by the output
  // stream manager.
  OutputStreamSpec* output_stream_spec_;
  std::list<Packet> output_queue_;
  bool closed_;
  Timestamp next_timestamp_bound_;
  // Equal to next_timestamp_bound_ only if the bound has been explicitly set
  // by the calculator.  This is needed for parallel Process() calls,
  // in order to avoid propagating the initial next_timestamp_bound_, which
  // does not reflect the output of Process() for preceding timestamps.
  Timestamp updated_next_timestamp_bound_;

  // Accesses OutputStreamShard for profiling.
  friend class GraphProfiler;
  // Accesses OutputStreamShard for profiling.
  friend class GraphTracer;
  // Accesses OutputStreamShard for profiling.
  friend class PerfettoTraceScope;
  // Accesses OutputStreamShard for post processing.
  friend class OutputStreamManager;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_OUTPUT_STREAM_SHARD_H_
