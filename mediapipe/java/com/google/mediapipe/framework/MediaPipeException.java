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

// Package java.nio.charset is not yet available in all Android apps.
import static com.google.common.base.Charsets.UTF_8;

/** This class represents an error reported by the MediaPipe framework. */
public class MediaPipeException extends RuntimeException {
  public MediaPipeException(int statusCode, String statusMessage) {
    super(StatusCode.values()[statusCode].description() + ": " + statusMessage);
    this.statusCode = StatusCode.values()[statusCode];
    this.statusMessage = statusMessage;
  }

  // Package base.Charsets is deprecated by package java.nio.charset is not
  // yet available in all Android apps.
  @SuppressWarnings("deprecation")
  MediaPipeException(int code, byte[] message) {
    this(code, new String(message, UTF_8));
  }

  public StatusCode getStatusCode() {
    return statusCode;
  }

  public String getStatusMessage() {
    return statusMessage;
  }

  /** The 18 canonical status codes. */
  public enum StatusCode {
    OK("ok"),
    CANCELLED("canceled"),
    UNKNOWN("unknown"),
    INVALID_ARGUMENT("invalid argument"),
    DEADLINE_EXCEEDED("deadline exceeded"),
    NOT_FOUND("not found"),
    ALREADY_EXISTS("already exists"),
    PERMISSION_DENIED("permission denied"),
    RESOURCE_EXHAUSTED("resource exhausted"),
    FAILED_PRECONDITION("failed precondition"),
    ABORTED("aborted"),
    OUT_OF_RANGE("out of range"),
    UNIMPLEMENTED("unimplemented"),
    INTERNAL("internal"),
    UNAVAILABLE("unavailable"),
    DATA_LOSS("data loss"),
    UNAUTHENTICATED("unauthenticated"),
    IO_EXCEPTION("i/o exception");

    StatusCode(String description) {
      this.description = description;
    }

    public String description() {
      return description;
    }

    private final String description;
  };

  private final StatusCode statusCode;
  private final String statusMessage;
}
