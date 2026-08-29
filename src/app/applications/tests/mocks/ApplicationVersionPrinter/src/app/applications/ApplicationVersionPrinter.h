#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_APPLICATIONVERSIONPRINTER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_APPLICATIONVERSIONPRINTER_CLASS_H

#include <gmock/gmock.h>

#include <memory>

#include "src/app/applications/IApplication.h"

namespace app
{

class ApplicationVersionPrinter : public IApplication
{
 public:
  virtual ~ApplicationVersionPrinter() = default;

  ApplicationVersionPrinter()
  {
    if (onMockCreate) {
      onMockCreate(*this);
    }
  }

  inline static std::function<void(ApplicationVersionPrinter&)> onMockCreate;

  MOCK_METHOD(int, run, (std::shared_ptr<ApplicationContext> ctx), (override));
};

}  // namespace app

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_APPLICATIONVERSIONPRINTER_CLASS_H
