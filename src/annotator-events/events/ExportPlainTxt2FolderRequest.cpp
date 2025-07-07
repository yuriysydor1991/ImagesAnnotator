#include "src/annotator-events/events/ExportPlainTxt2FolderRequest.h"

#include <string>

namespace events::events
{

ExportPlainTxt2FolderRequest::ExportPlainTxt2FolderRequest(
    const std::string& newFolderPath)
    : dst_folder_path{newFolderPath}
{
}

}  // namespace events::events
