#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IEVENT_ABSTRACT_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IEVENT_ABSTRACT_CLASS_H

namespace events::events
{

/**
 * @brief The general abstract class for all event
 */
class IEvent
{
 public:
  virtual ~IEvent() = default;
  IEvent() = default;
};

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IEVENT_ABSTRACT_CLASS_H
