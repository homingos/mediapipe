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

#include "mediapipe/util/rectangle_util.h"

#include "mediapipe/framework/formats/rect.pb.h"
#include "mediapipe/framework/port/rectangle.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/port/statusor.h"

namespace mediapipe {

// Converts a NormalizedRect into a Rectangle_f.
absl::StatusOr<Rectangle_f> ToRectangle(
    const mediapipe::NormalizedRect& input) {
  if (!input.has_x_center() || !input.has_y_center() || !input.has_width() ||
      !input.has_height()) {
    return absl::InvalidArgumentError("Missing dimensions in NormalizedRect.");
  }
  if (input.width() < 0.0f || input.height() < 0.0f) {
    return absl::InvalidArgumentError("Negative rectangle width or height.");
  }

  const float xmin = input.x_center() - input.width() / 2.0;
  const float ymin = input.y_center() - input.height() / 2.0;

  // TODO: Support rotation for rectangle.
  return Rectangle_f(xmin, ymin, input.width(), input.height());
}

// If the new_rect overlaps with any of the rectangles in
// existing_rects, then return true. Otherwise, return false.
absl::StatusOr<bool> DoesRectOverlap(
    const mediapipe::NormalizedRect& new_rect,
    absl::Span<const mediapipe::NormalizedRect> existing_rects,
    float min_similarity_threshold) {
  MP_ASSIGN_OR_RETURN(Rectangle_f new_rectangle, ToRectangle(new_rect));

  for (const mediapipe::NormalizedRect& existing_rect : existing_rects) {
    MP_ASSIGN_OR_RETURN(Rectangle_f existing_rectangle,
                        ToRectangle(existing_rect));
    if (CalculateIou(existing_rectangle, new_rectangle) >
        min_similarity_threshold) {
      return true;
    }
  }
  return false;
}

// Computes the overlap similarity based on Intersection over Union (IoU) of
// two rectangles.  Result is bounded between [0.0, 1.0], where 0.0 means no
// intersection at all, and 1.0 means the two rectangles are identical.
float CalculateIou(const Rectangle_f& rect1, const Rectangle_f& rect2) {
  if (!rect1.Intersects(rect2)) return 0.0f;

  // Compute IoU similarity score.
  const float intersection_area = Rectangle_f(rect1).Intersect(rect2).Area();
  const float normalization = rect1.Area() + rect2.Area() - intersection_area;
  return normalization > 0.0f ? intersection_area / normalization : 0.0f;
}

}  // namespace mediapipe
