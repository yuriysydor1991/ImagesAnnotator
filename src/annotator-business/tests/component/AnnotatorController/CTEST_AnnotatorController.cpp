#include "CTEST_AnnotatorController.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "ctests_decls.h"
#include "src/annotator-business/AnnotatorController.h"
#include "src/annotator-events/ImagesAnnotatorEventController.h"
#include "src/app/ApplicationContext.h"
#include "src/log/log.h"

CTEST_AnnotatorController::CTEST_AnnotatorController()
    : actx{create_app_context(argc, argv)},
      controller{std::make_shared<AnnotatorController>()}
{
  EXPECT_NE(controller, nullptr);
  EXPECT_NE(actx, nullptr);
}

inline std::shared_ptr<app::ApplicationContext>
CTEST_AnnotatorController::create_app_context(int& argc, char**& argv)
{
  auto c = std::make_shared<app::ApplicationContext>(argc, argv);

  c->eventer = std::make_shared<events::ImagesAnnotatorEventController>(c);

  return c;
}