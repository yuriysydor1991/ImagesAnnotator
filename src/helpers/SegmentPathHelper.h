/**
 * The "Simplified BSD License"
 *
 * Copyright (c) 2026, Yurii Sydor (yuriysydor1991@gmail.com)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef IMAGES_ANNOTATOR_PROJECT_SEGMENT_PATH_HELPER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_SEGMENT_PATH_HELPER_CLASS_H

#include <cstddef>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"
#include "src/annotator-events/events/ImageRecordSegment.h"
#include "src/helpers/IHelper.h"

namespace helpers
{

/**
 * @brief The helper class of the segment paths every GUI toolkit draws and
 * edits the same way.
 *
 * A segment is drawn by the left mouse button clicks made while the Shift key
 * is held, every click appending a point to its path, and the release of the
 * Shift key loops the path into the closed polygon. Afterwards every single
 * point of the polygon is dragged to a new position on its own.
 *
 * The ImageRecordSegment of the ImagesAnnotator-DataDrivers library keeps its
 * points in the 0..1 space of its bounding rectangle, so a point moved in the
 * image pixel space refits the rectangle to the whole polygon here, the same
 * way the add_point() of the record does for an appended point.
 */
class SegmentPathHelper : virtual public IHelper
{
 public:
  using ImageRecordPtr = events::events::ImageRecordPtr;
  using ImageRecordRectPtr = events::events::ImageRecordRectPtr;
  using ImageRecordSegment = events::events::ImageRecordSegment;
  using ImageRecordSegmentPtr = events::events::ImageRecordSegmentPtr;
  using Point = ImageRecordSegment::Point;
  using Points = ImageRecordSegment::Points;

  /// @brief The point index reported when no point is found
  inline static constexpr const std::size_t npos = static_cast<std::size_t>(-1);

  /// @brief The distance in the canvas pixels a point is grabbed within
  inline static constexpr const double grab_distance = 6.0;

  virtual ~SegmentPathHelper() = default;
  SegmentPathHelper() = default;

  /**
   * @brief Reports the given annotation as a segment.
   *
   * @return Returns the segment, or nullptr for a plain rectangle.
   */
  static ImageRecordSegmentPtr as_segment(const ImageRecordRectPtr& rect);

  /**
   * @brief Reports every point of the segment path in the image pixel space,
   * also of the open path of less than ImageRecordSegment::min_points, whose
   * outline() reports the rectangle corners instead.
   */
  static Points image_points(const ImageRecordSegment& segment);

  /**
   * @brief Replaces the segment path with the given points of the image pixel
   * space and makes the rectangle the bounding box of them.
   */
  static void set_image_points(ImageRecordSegment& segment,
                               const Points& image);

  /**
   * @brief Moves the point of the given index to the given position of the
   * image pixel space and refits the rectangle to the whole path.
   *
   * @return Returns false for an index out of the path.
   */
  static bool move_point(ImageRecordSegment& segment, const std::size_t& index,
                         const double& px, const double& py);

  /**
   * @brief Finds the point of the segment path nearest to the given canvas
   * position.
   *
   * @param cx The canvas x of the position.
   * @param cy The canvas y of the position.
   * @param scale The preview scale the canvas draws the image with.
   * @param distance The canvas distance the point is looked for within.
   *
   * @return Returns the point index, or npos when none lies within the
   * distance.
   */
  static std::size_t find_point(const ImageRecordSegment& segment,
                                const double& cx, const double& cy,
                                const double& scale,
                                const double& distance = grab_distance);

  /**
   * @brief Converts a canvas position into the image pixel space, clamped to
   * the image of the given canvas size.
   */
  static Point to_image_point(const double& cx, const double& cy,
                              const double& canvasWidth,
                              const double& canvasHeight, const double& scale);

  /**
   * @brief Tells whether the segment path has the points enough to loop it into
   * a polygon.
   */
  static bool is_closable(const ImageRecordSegment& segment);

  /**
   * @brief Finishes the drawing of the segment: a path of
   * ImageRecordSegment::min_points or more is the closed polygon already,
   * while a shorter one, which outlines nothing, is erased from the
   * annotations of the image record (and stops being its current one).
   *
   * @return Returns true when the segment is kept.
   */
  static bool finish(const ImageRecordPtr& ir,
                     const ImageRecordSegmentPtr& segment);
};

}  // namespace helpers

#endif  // IMAGES_ANNOTATOR_PROJECT_SEGMENT_PATH_HELPER_CLASS_H
