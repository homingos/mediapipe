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

#ifndef MEDIAPIPE_DEPS_CLOCK_H_
#define MEDIAPIPE_DEPS_CLOCK_H_

#include "absl/time/time.h"

namespace mediapipe {

// An abstract interface representing a Clock, which is an object that can
// tell you the current time, and sleep.
//
// This interface allows decoupling code that uses time from the code that
// creates a point in time.  You can use this to your advantage by injecting
// Clocks into interfaces rather than having implementations call absl::Now()
// directly.
//
// The Clock::RealClock() function returns a pointer (that you do not own)
// to the global realtime clock.
//
// Example:
//
//   bool IsWeekend(Clock* clock) {
//     absl::Time now = clock->TimeNow();
//     // ... code to check if 'now' is a weekend.
//   }
//
//   // Production code.
//   IsWeekend(Clock::RealClock());
//
//   // Test code:
//   MyTestClock test_clock(SATURDAY);
//   IsWeekend(&test_clock);
//
class Clock {
 public:
  // Returns a pointer to the global realtime clock.  The caller does not
  // own the returned pointer and should not delete it.  The returned clock
  // is thread-safe.
  static Clock* RealClock();

  virtual ~Clock();

  // Returns the current time.
  virtual absl::Time TimeNow() = 0;

  // Sleeps for the specified duration.
  virtual void Sleep(absl::Duration d) = 0;

  // Sleeps until the specified time.
  virtual void SleepUntil(absl::Time wakeup_time) = 0;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_DEPS_CLOCK_H_
