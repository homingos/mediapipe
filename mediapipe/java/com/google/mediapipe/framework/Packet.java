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

package com.google.mediapipe.framework;

/**
 * Java wrapper class for a native MediaPipe Packet.
 *
 * <p>To interpret the content of the packet, use {@link PacketGetter}. To create content of a
 * packet, use {@link PacketCreator}. Java Packet should be released manually when no longer needed.
 *
 * <p>{@link Packet} can also be managed by {@link Graph}, which automatically releases all the
 * packets in the context, however, we still need to be careful of the memory, and release them as
 * soon as not needed.
 */
public class Packet {
  // Points to a native Packet.
  private long nativePacketHandle;

  /**
   * Creates a Java packet from a native mediapipe packet handle.
   *
   * Returns a Packet from a native internal::PacketWithContext handle.
   */
  public static Packet create(long nativeHandle) {
    return new Packet(nativeHandle);
  }

  /**
   * Returns the native handle of the packet.
   */
  public long getNativeHandle() {
    return nativePacketHandle;
  }

  /** Returns the timestamp of the Packet. */
  public long getTimestamp() {
    return nativeGetTimestamp(nativePacketHandle);
  }

  /** Returns true if the Packet is empty. */
  public boolean isEmpty() {
    return nativeIsEmpty(nativePacketHandle);
  }

  /**
   * Returns a shared copy of the Packet.
   *     <p>This is essentially increasing the reference count to the data encapsulated in the
   *     native mediapipe packet.
   */
  public Packet copy() {
    return new Packet(nativeCopyPacket(nativePacketHandle));
  }

  /**
   * Releases the native allocation of the packet.
   *
   * <p>After the Graph for this packet is torn down, calling this will cause unexpected behavior.
   * Since Graph tearDown will release all native memories of the Packets it holds.
   */
  public void release() {
    if (nativePacketHandle != 0) {
      nativeReleasePacket(nativePacketHandle);
      nativePacketHandle = 0;
    }
  }

  // Packet is not intended to be constructed directly.
  private Packet(long handle) {
    nativePacketHandle = handle;
  }

  // Releases the native memeory.
  private native void nativeReleasePacket(long packetHandle);

  private native long nativeCopyPacket(long packetHandle);

  private native long nativeGetTimestamp(long packetHandle);

  private native boolean nativeIsEmpty(long packetHandle);
}
