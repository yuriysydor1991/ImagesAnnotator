#include "ApplicationContext.h"

namespace app
{

ApplicationContext::ApplicationContext(int& gargc, char**& gargv)
    : argc{gargc}, argv{gargv}
{
}

bool ApplicationContext::conversion_requested() const
{
  return !convert_from.empty() && !convert_src.empty() && !convert_to.empty() &&
         !convert_dst.empty();
}

}  // namespace app