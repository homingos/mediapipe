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

#ifndef MEDIAPIPE_FRAMEWORK_PACKET_SET_H_
#define MEDIAPIPE_FRAMEWORK_PACKET_SET_H_

#include "mediapipe/framework/collection.h"
#include "mediapipe/framework/packet.h"

namespace mediapipe {

// A PacketSet is used to hold a collection of Packets accessed either
// by index or by tag name.
typedef internal::Collection<Packet> PacketSet;

// A similar construct for output side packets.
class OutputSidePacket;
typedef internal::Collection<OutputSidePacket,
                             internal::CollectionStorage::kStorePointer>
    OutputSidePacketSet;

// Similar constructs for input and output streams.
// TODO: Remove InputStreamSet and OutputStreamSet.
class InputStream;
typedef internal::Collection<InputStream*> InputStreamSet;
class OutputStream;
typedef internal::Collection<OutputStream*> OutputStreamSet;

// Similar constructs for input and output stream shards.
// TODO: Rename to InputStreamSet and OutputStreamSet.
class InputStreamShard;
typedef internal::Collection<InputStreamShard> InputStreamShardSet;
class OutputStreamShard;
typedef internal::Collection<OutputStreamShard> OutputStreamShardSet;

}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_PACKET_SET_H_
