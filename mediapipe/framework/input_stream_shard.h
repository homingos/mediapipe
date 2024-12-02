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

#ifndef MEDIAPIPE_FRAMEWORK_INPUT_STREAM_SHARD_H_
#define MEDIAPIPE_FRAMEWORK_INPUT_STREAM_SHARD_H_

#include <queue>
#include <string>
#include <utility>

#include "mediapipe/framework/input_stream.h"
#include "mediapipe/framework/packet.h"

namespace mediapipe {

// For testing
class MediaPipeProfilerTestPeer;

// InputStreamShard, a subclass of InputStream, holds a header packet, a FIFO
// queue of input packets, and a bool variable to indicate if the stream is
// completely done. Each call to Calculator::Open(), Calculator::Process(), and
// Calculator::Close() can only access its own InputStreamShard(s).
//
// The input stream handler makes sure exactly one packet is added to each shard
// per Calculator::Process call. This is done by pushing empty packets when
// necessary to guarantee alignment with the corresponding timestamps. Every
// call to ClearCurrentPacket() must remove a packet from the queue and every
// call to Value() must successfully return the front element of the queue.
class InputStreamShard : public InputStream {
 public:
  InputStreamShard() : is_done_(false) {}

  // Returns the first packet in the queue if there is any, otherwise returns an
  // empty packet.
  const Packet& Value() const override {
    return !packet_queue_.empty() ? packet_queue_.front() : empty_packet_;
  }

  Packet& Value() override {
    return !packet_queue_.empty() ? packet_queue_.front() : empty_packet_;
  }

  // Returns a reference to the name string of the InputStreamManager.
  const std::string& Name() const { return *name_; }

  bool IsDone() const override { return is_done_; }

 private:
  void SetName(const std::string* name) { name_ = name; }

  int NumberOfPackets() const { return static_cast<int>(packet_queue_.size()); }

  void ClearCurrentPacket() {
    if (!packet_queue_.empty()) {
      packet_queue_.pop();
    }
  }

  void SetHeader(const Packet& header) { header_ = header; }

  void AddPacket(Packet&& value, bool is_done);

  // Packet storage for batch processing.
  std::queue<Packet> packet_queue_;
  Packet empty_packet_;

  // Pointer to the name string of the InputStreamManager.
  const std::string* name_;
  bool is_done_;

  // Accesses InputStreamShard for setting data.
  friend class InputStreamHandler;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_INPUT_STREAM_SHARD_H_
