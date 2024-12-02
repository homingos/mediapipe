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

#include "mediapipe/util/tracking/box_tracker.h"

#include "mediapipe/framework/deps/file_path.h"
#include "mediapipe/framework/port/gtest.h"

namespace mediapipe {
namespace {

constexpr double kWidth = 1280.0;
constexpr double kHeight = 720.0;

// Ground truth test; testing tracking accuracy and multi-thread load testing.
TEST(BoxTrackerTest, MovingBoxTest) {
  const std::string cache_dir =
      file::JoinPath("./", "/mediapipe/util/tracking/testdata/box_tracker");
  BoxTracker box_tracker(cache_dir, BoxTrackerOptions());

  // Ground truth positions of the overlay (linear in between).
  // @ 0:     (50, 100)
  // @ 3000:  (50, 400)
  // @ 6000:  (500, 400)
  // @ 9000:  (1000, 50)
  // @ 12000: (50, 100)
  // @ 15000: (1000, 400)
  //
  // size of overlay: 220 x 252
  std::vector<Vector2_d> positions{
      {50.0 / kWidth, 100.0 / kHeight},  {50.0 / kWidth, 400.0 / kHeight},
      {500.0 / kWidth, 400.0 / kHeight}, {1000.0 / kWidth, 50.0 / kHeight},
      {50.0 / kWidth, 100.0 / kHeight},  {1000.0 / kWidth, 400.0 / kHeight},
  };

  const Vector2_d overlay_sz(220.0 / kWidth, 252.0 / kHeight);

  TimedBox initial_pos;
  initial_pos.left = positions[1].x();
  initial_pos.top = positions[1].y();
  initial_pos.right = initial_pos.left + overlay_sz.x();
  initial_pos.bottom = initial_pos.top + overlay_sz.y();
  initial_pos.time_msec = 3000;

  // Test multithreading under load, ensure this does not crash or stall.
  box_tracker.NewBoxTrack(initial_pos, 0);
  // Cancel right after issuing.
  box_tracker.CancelAllOngoingTracks();

  // Should not be scheduled.
  box_tracker.NewBoxTrack(initial_pos, 0);
  EXPECT_FALSE(box_tracker.IsTrackingOngoing());
  box_tracker.ResumeTracking();

  box_tracker.NewBoxTrack(initial_pos, 0);
  // Two cancelations in a row should not block.
  box_tracker.CancelAllOngoingTracks();
  box_tracker.CancelAllOngoingTracks();
  box_tracker.ResumeTracking();

  // Start again for real this time.
  box_tracker.NewBoxTrack(initial_pos, 0);

  // Wait to terminate.
  box_tracker.WaitForAllOngoingTracks();

  // Check that tracking did not abort.
  EXPECT_EQ(0, box_tracker.TrackInterval(0).first);
  EXPECT_GT(box_tracker.TrackInterval(0).second, 15000);

  auto boxes_equal = [](const TimedBox& lhs, const TimedBox& rhs) {
    constexpr float kAccuracy = 0.015f;
    return (lhs.time_msec == rhs.time_msec &&
            std::abs(lhs.top - rhs.top) < kAccuracy &&
            std::abs(lhs.left - rhs.left) < kAccuracy &&
            std::abs(lhs.right - rhs.right) < kAccuracy &&
            std::abs(lhs.bottom - rhs.bottom) < kAccuracy);
  };

  for (int k = 0; k < 15000; k += 33) {
    TimedBox box;
    EXPECT_TRUE(box_tracker.GetTimedPosition(0, k, &box));

    // One groundtruth position every 3s, linear in between.
    const int rect_pos = k / 3000;
    const int dt = k - rect_pos * 3000;
    const int next_rect_pos = dt == 0 ? rect_pos : (rect_pos + 1);

    // Blend to get initial position.
    const double alpha = dt / 3000.0;
    const Vector2_d gt_pos =
        (1.0 - alpha) * positions[rect_pos] + alpha * positions[next_rect_pos];

    TimedBox gt_box;
    gt_box.time_msec = k;
    gt_box.top = gt_pos.y();
    gt_box.left = gt_pos.x();
    gt_box.right = gt_box.left + overlay_sz.x();
    gt_box.bottom = gt_box.top + overlay_sz.y();
    EXPECT_TRUE(boxes_equal(gt_box, box));
  }
}

}  // namespace

}  // namespace mediapipe
