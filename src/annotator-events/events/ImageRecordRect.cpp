/**
 * The "Simplified BSD License"
 *
 * Copyright (c) 2025, Yurii Sydor (yuriysydor1991@gmail.com)
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

#include "src/annotator-events/events/ImageRecordRect.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <string>
#include <unordered_set>

#include "src/annotator-events/events/IRecord.h"
#include "src/log/log.h"

namespace events::events
{

ImageRecordRect::ImageRecordRect() {}

ImageRecordRect::ImageRecordRect(const std::string& nname, const int& nx,
                                 const int& ny, const int& nwidth,
                                 const int& nheight)
    : name{nname}, x{nx}, y{ny}, width{nwidth}, height{nheight}
{
}

ImageRecordRectPtr ImageRecordRect::duplicate_shared() const
{
  return std::make_shared<ImageRecordRect>(*this);
}

bool ImageRecordRect::equal(const ImageRecordRectSet& l,
                            const ImageRecordRectSet& r)
{
  if (&l == &r) {
    LOGT("The same object given as parameters");
    return true;
  }

  const bool rt = std::equal(
      l.begin(), l.end(), r.begin(), r.end(),
      [](const ImageRecordRectPtr& lirr, const ImageRecordRectPtr& rirr) {
        return equal(lirr, rirr);
      });

  LOGT("Equality value: " << rt);

  return rt;
}

bool ImageRecordRect::equal(const ImageRecordRectPtr& l,
                            const ImageRecordRectPtr& r)
{
  if (l.get() == r.get()) {
    LOGT("The same object given");
    return true;
  }

  LOGT("Comparing: " << l->name << " ?= " << r->name);
  LOGT("         : " << l->x << " ?= " << r->x);
  LOGT("         : " << l->y << " ?= " << r->y);
  LOGT("         : " << l->width << " ?= " << r->width);
  LOGT("         : " << l->height << " ?= " << r->height);

  const bool rt = l->name == r->name && l->x == r->x && l->y == r->y &&
                  l->width == r->width && l->height == r->height;

  LOGT("Equality value for " << l->name << " = " << rt);

  return rt;
}

}  // namespace events::events
