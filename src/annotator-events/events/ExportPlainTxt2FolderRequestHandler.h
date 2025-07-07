#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_EXPORTPLAINTXT2FOLDERREQUESTHANDLER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_EXPORTPLAINTXT2FOLDERREQUESTHANDLER_CLASS_H

#include <memory>

#include "src/annotator-events/events/EventIHandler.h"
#include "src/annotator-events/events/ExportPlainTxt2FolderRequest.h"
#include "src/annotator-events/events/ImagesDirChanged.h"

namespace events::events
{

/**
 * @brief The plain txt format to folder export event handler abstract class.
 */
class ExportPlainTxt2FolderRequestHandler : virtual public EventIHandler
{
 public:
  virtual ~ExportPlainTxt2FolderRequestHandler() = default;
  ExportPlainTxt2FolderRequestHandler() = default;

  virtual void handle(ExportPlainTxt2FolderRequestPtr event) = 0;
};

using ExportPlainTxt2FolderRequestHandlerPtr =
    std::shared_ptr<ExportPlainTxt2FolderRequestHandler>;

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_EXPORTPLAINTXT2FOLDERREQUESTHANDLER_CLASS_H
