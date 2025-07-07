#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_EXPORTPLAINTXT2FOLDERREQUEST_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_EXPORTPLAINTXT2FOLDERREQUEST_CLASS_H

#include <memory>
#include <string>

#include "src/annotator-events/events/IRequestEvent.h"

namespace events::events
{

/**
 * @brief Even for available annotations data export as a plain txt to some
 * folder.
 */
class ExportPlainTxt2FolderRequest : virtual public IRequestEvent
{
 public:
  virtual ~ExportPlainTxt2FolderRequest() = default;
  explicit ExportPlainTxt2FolderRequest(const std::string& newFolderPath);

  std::string dst_folder_path;
};

using ExportPlainTxt2FolderRequestPtr =
    std::shared_ptr<ExportPlainTxt2FolderRequest>;

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_EXPORTPLAINTXT2FOLDERREQUEST_CLASS_H
