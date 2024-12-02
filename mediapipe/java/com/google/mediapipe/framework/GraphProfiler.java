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

import com.google.common.base.Preconditions;
import com.google.mediapipe.proto.CalculatorProfileProto.CalculatorProfile;
import com.google.protobuf.InvalidProtocolBufferException;
import java.util.ArrayList;
import java.util.List;

/** MediaPipe Profiler Java API. */
public class GraphProfiler {
  private final long nativeProfilerHandle;
  private final Graph mediapipeGraph;

  GraphProfiler(long nativeProfilerHandle, Graph mediapipeGraph) {
    Preconditions.checkState(
          nativeProfilerHandle != 0,
          "Invalid profiler, tearDown() might have been called already.");
    this.nativeProfilerHandle = nativeProfilerHandle;
    this.mediapipeGraph = mediapipeGraph;
  }

  /**
   * Resets all the calculator profilers in the graph. This only resets the information about
   * Process() and does NOT affect information for Open() and Close() methods.
   */
  public void reset() {
    synchronized (mediapipeGraph) {
      checkContext();
      nativeReset(nativeProfilerHandle);
    }
  }

  /** Resumes all the calculator profilers in the graph. No-op if already profiling. */
  public void resume() {
    synchronized (mediapipeGraph) {
      checkContext();
      nativeResume(nativeProfilerHandle);
    }
  }

  /** Pauses all the calculator profilers in the graph. No-op if already paused. */
  public void pause() {
    synchronized (mediapipeGraph) {
      checkContext();
      nativePause(nativeProfilerHandle);
    }
  }

  /**
   * Collects the runtime profile for Open(), Process(), and Close() of each calculator in the
   * graph. May be called at any time after the graph has been initialized.
   */
  public List<CalculatorProfile> getCalculatorProfiles() {
    synchronized (mediapipeGraph) {
      checkContext();
      byte[][] profileBytes = nativeGetCalculatorProfiles(nativeProfilerHandle);
      List<CalculatorProfile> profileList = new ArrayList<>();
      if (profileBytes == null) {
        return profileList;
      }
      for (byte[] element : profileBytes) {
        try {
          CalculatorProfile profile = CalculatorProfile.parseFrom(element);
          profileList.add(profile);
        } catch (InvalidProtocolBufferException e) {
          throw new RuntimeException(e);
        }
      }
      return profileList;
    }
  }

  private void checkContext() {
    Preconditions.checkState(
        mediapipeGraph.getNativeHandle() != 0,
        "Invalid context, tearDown() might have been called already.");
  }

  private native void nativeReset(long profilingContextHandle);

  private native void nativeResume(long profilingContextHandle);

  private native void nativePause(long profilingContextHandle);

  private native byte[][] nativeGetCalculatorProfiles(long profilingContextHandle);
}
