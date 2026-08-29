#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_APPLICATIONHELPPRINTER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_APPLICATIONHELPPRINTER_CLASS_H

#include <gmock/gmock.h>

#include <functional>
#include <memory>

#include "src/app/applications/IApplication.h"

namespace app
{

class ApplicationHelpPrinter : public IApplication
{
 public:
  virtual ~ApplicationHelpPrinter() = default;

  ApplicationHelpPrinter()
  {
    if (onMockCreate) {
      onMockCreate(*this);
    }
  }

  inline static std::function<void(ApplicationHelpPrinter&)> onMockCreate;

  MOCK_METHOD(int, run, (std::shared_ptr<ApplicationContext> ctx), (override));
};

}  // namespace app

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_APPLICATIONHELPPRINTER_CLASS_H
