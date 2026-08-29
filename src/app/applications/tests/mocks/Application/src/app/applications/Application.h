#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_APPLICATION_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_APPLICATION_CLASS_H

#include <gmock/gmock.h>

#include <functional>
#include <memory>

#include "src/app/ApplicationContext.h"
#include "src/app/applications/IApplication.h"

namespace app
{

class Application : public IApplication
{
 public:
  virtual ~Application() = default;

  Application()
  {
    if (onMockCreate) {
      onMockCreate(*this);
    }
  }

  inline static std::function<void(Application&)> onMockCreate;

  MOCK_METHOD(int, run, (std::shared_ptr<ApplicationContext> ctx), (override));
};

}  // namespace app

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_APPLICATION_CLASS_H
