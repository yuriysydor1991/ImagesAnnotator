#ifndef IMAGES_ANNOTATOR_PROJECT_IHELPER_ABSTRACT_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_IHELPER_ABSTRACT_CLASS_H

/**
 * @brief The namespace to contain all the entities for
 * the helpers general purpose classes.
 */
namespace helpers
{

/**
 * @brief The abstract class that will be parent for all helpers.
 */
class IHelper
{
 public:
  virtual ~IHelper() = default;
  IHelper() = default;
};

}  // namespace helpers

#endif  // IMAGES_ANNOTATOR_PROJECT_IHELPER_ABSTRACT_CLASS_H
