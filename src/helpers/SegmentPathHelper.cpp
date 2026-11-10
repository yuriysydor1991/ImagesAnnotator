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

#include "src/helpers/SegmentPathHelper.h"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <memory>
#include <tuple>

#include "src/log/log.h"

namespace helpers
{

namespace
{
double share(const double& offset, const int& size)
{
  return size == 0 ? 0.0 : offset / size;
}

int floored(const double& value) { return static_cast<int>(std::floor(value)); }

int ceiled(const double& value) { return static_cast<int>(std::ceil(value)); }
}  // namespace

SegmentPathHelper::ImageRecordSegmentPtr SegmentPathHelper::as_segment(
    const ImageRecordRectPtr& rect)
{
  return std::dynamic_pointer_cast<ImageRecordSegment>(rect);
}

SegmentPathHelper::Points SegmentPathHelper::image_points(
    const ImageRecordSegment& segment)
{
  Points image;

  image.reserve(segment.points.size());

  std::transform(segment.points.cbegin(), segment.points.cend(),
                 std::back_inserter(image),
                 [&segment](const Point& p) -> Point {
                   const auto& [px, py] = p;
                   return {segment.x + px * segment.width,
                           segment.y + py * segment.height};
                 });

  return image;
}

void SegmentPathHelper::set_image_points(ImageRecordSegment& segment,
                                         const Points& image)
{
  segment.points.clear();

  if (image.empty()) {
    LOGT("No points to fit the segment rectangle to");
    return;
  }

  const auto [left, right] = std::minmax_element(
      image.cbegin(), image.cend(), [](const Point& l, const Point& r) {
        return std::get<0>(l) < std::get<0>(r);
      });
  const auto [top, bottom] = std::minmax_element(
      image.cbegin(), image.cend(), [](const Point& l, const Point& r) {
        return std::get<1>(l) < std::get<1>(r);
      });

  segment.x = floored(std::get<0>(*left));
  segment.y = floored(std::get<1>(*top));
  segment.width = ceiled(std::get<0>(*right)) - segment.x;
  segment.height = ceiled(std::get<1>(*bottom)) - segment.y;

  segment.points.reserve(image.size());

  for (const auto& [px, py] : image) {
    segment.points.emplace_back(share(px - segment.x, segment.width),
                                share(py - segment.y, segment.height));
  }
}

bool SegmentPathHelper::move_point(ImageRecordSegment& segment,
                                   const std::size_t& index, const double& px,
                                   const double& py)
{
  if (index >= segment.points.size()) {
    LOGE("No segment point of the index " << index);
    return false;
  }

  Points image = image_points(segment);

  image[index] = Point{px, py};

  set_image_points(segment, image);

  return true;
}

std::size_t SegmentPathHelper::find_point(const ImageRecordSegment& segment,
                                          const double& cx, const double& cy,
                                          const double& scale,
                                          const double& distance)
{
  std::size_t found = npos;
  double nearest = distance * distance;

  const Points image = image_points(segment);

  for (std::size_t index = 0U; index < image.size(); ++index) {
    const auto& [px, py] = image[index];

    const double dx = px * scale - cx;
    const double dy = py * scale - cy;
    const double squared = dx * dx + dy * dy;

    if (squared <= nearest) {
      nearest = squared;
      found = index;
    }
  }

  return found;
}

SegmentPathHelper::Point SegmentPathHelper::to_image_point(
    const double& cx, const double& cy, const double& canvasWidth,
    const double& canvasHeight, const double& scale)
{
  if (scale <= 0.0) {
    LOGE("Invalid image scale " << scale);
    return Point{0.0, 0.0};
  }

  return Point{std::clamp(cx, 0.0, std::max(canvasWidth, 0.0)) / scale,
               std::clamp(cy, 0.0, std::max(canvasHeight, 0.0)) / scale};
}

bool SegmentPathHelper::is_closable(const ImageRecordSegment& segment)
{
  return segment.points.size() >= ImageRecordSegment::min_points;
}

bool SegmentPathHelper::finish(const ImageRecordPtr& ir,
                               const ImageRecordSegmentPtr& segment)
{
  if (segment == nullptr) {
    LOGT("No segment to finish");
    return false;
  }

  if (is_closable(*segment)) {
    LOGT("The segment path of " << segment->points.size()
                                << " points is looped into a polygon");
    return true;
  }

  LOGD("Dropping the segment path of " << segment->points.size()
                                       << " points, less than "
                                       << ImageRecordSegment::min_points);

  if (ir == nullptr) {
    return false;
  }

  const ImageRecordRectPtr rect = segment;

  auto& rects = ir->rects;

  rects.erase(std::remove(rects.begin(), rects.end(), rect), rects.end());

  if (ir->current_rect == rect) {
    ir->current_rect.reset();
  }

  return false;
}

}  // namespace helpers
