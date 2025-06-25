#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IPROVIDER_ABSTRACT_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IPROVIDER_ABSTRACT_CLASS_H

namespace events::events
{

/**
 * @brief The general abstract class for all data providers.
 */
class IProvider
{
 public:
  virtual ~IProvider() = default;
  IProvider() = default;
};

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IPROVIDER_ABSTRACT_CLASS_H
