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
 * Implement this interface to wrap a native GraphService.
 *
 * <p>T should be the Java class wrapping the native service object.
 */
public interface GraphService<T> {
  /**
   * Provides the native service object corresponding to the provided Java object. This must be
   * handled by calling mediapipe::android::GraphServiceHelper::SetServiceObject in native code,
   * passing the provided context argument. We do it this way to minimize the number of trips
   * through JNI and maintain more type safety in the native code.
   */
  public void installServiceObject(long context, T object);
}
