#ifndef IMAGES_ANNOTATOR_PROJECT_TYPE_HELPER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_TYPE_HELPER_CLASS_H

#include <cmath>

#include "src/helpers/IHelper.h"

namespace helpers
{

/**
 * @brief The helper class to shorted the static_cast<> expressions in the code
 */
class TypeHelper : virtual public IHelper
{
 public:
  virtual ~TypeHelper() = default;
  TypeHelper() = default;

  template <class Ntype>
  inline static double toD(const Ntype& val)
  {
    return static_cast<double>(val);
  }

  template <class Ntype>
  inline static int ceilInt(const Ntype& val)
  {
    const auto ceiledV = std::ceil(val);

    return static_cast<int>(ceiledV);
  }

  template <class Ntype>
  inline static int toI(const Ntype& val)
  {
    return static_cast<int>(val);
  }
};

}  // namespace helpers

#endif  // IMAGES_ANNOTATOR_PROJECT_TYPE_HELPER_CLASS_H
