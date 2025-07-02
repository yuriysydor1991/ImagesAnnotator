#include "src/annotator-events/events/StoreRequest.h"

#include <string>

namespace events::events
{

StoreRequest::StoreRequest(const std::string& newdbpath) : dbpath{newdbpath} {}

}  // namespace events::events
