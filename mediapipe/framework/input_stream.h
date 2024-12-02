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
// Defines InputStream, from which Calculator implementations get input
// packets.

#ifndef MEDIAPIPE_FRAMEWORK_INPUT_STREAM_H_
#define MEDIAPIPE_FRAMEWORK_INPUT_STREAM_H_

#include <string>

#include "absl/base/macros.h"
#include "mediapipe/framework/packet.h"
#include "mediapipe/framework/port.h"

namespace mediapipe {

class InputStream {
 public:
  InputStream(const InputStream&) = delete;
  InputStream& operator=(const InputStream&) = delete;

  // Returns the const reference of the input, if the input is present, or an
  // empty Packet otherwise.
  virtual const Packet& Value() const = 0;

  // Returns the non-const reference of the input, if the input is present, or
  // an empty Packet otherwise.
  virtual Packet& Value() = 0;

  // Syntactic sugar for a common way to get the typed value from the stream.
  template <typename T>
  const T& Get() const {
    return Value().Get<T>();
  }

  // Syntactic sugar for checking if the input is empty.
  bool IsEmpty() const { return Value().IsEmpty(); }

  // Returns true iff the Inputstream has been closed and there are no remaining
  // Packets queued for processing. (Note that there may currently be a Packet
  // available from the stream inside a Calculator's Process() function.)
  // The stream could be closed either because the corresponding
  // OutputStream::Close() has been called by the upstream Calculator, or
  // because that Calculator has itself been Close()d. It is guaranteed that
  // IsDone()==true when called from Calculator::Close().
  virtual bool IsDone() const = 0;

  // Returns the const reference of the header Packet, if one was set for the
  // corresponding OutputStream in the upstream Calculator's Open method.
  // May be called in Calculator::Open(), Process() or Close().
  const Packet& Header() const { return header_; }

 protected:
  InputStream() = default;
  virtual ~InputStream() = default;

  Packet header_;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_INPUT_STREAM_H_
