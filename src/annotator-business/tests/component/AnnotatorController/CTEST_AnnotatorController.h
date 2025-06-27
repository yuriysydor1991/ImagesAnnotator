#ifndef IMAGEANNOTATOR_GTKMM3_PROJECT_ANNOTATORCOTNROLLER_CT_BASE_CLASS_H
#define IMAGEANNOTATOR_GTKMM3_PROJECT_ANNOTATORCOTNROLLER_CT_BASE_CLASS_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "ctests_decls.h"
#include "src/annotator-business/AnnotatorController.h"
#include "src/annotator-events/ImagesAnnotatorEventController.h"
#include "src/app/ApplicationContext.h"
#include "src/log/log.h"

using namespace testing;
using namespace iannotator;
using namespace events::events;

class CTEST_AnnotatorController : public Test
{
 public:
  ~CTEST_AnnotatorController() = default;
  CTEST_AnnotatorController();

  static std::shared_ptr<app::ApplicationContext> create_app_context(
      int& argc, char**& argv);

  int argc{0};
  char** argv{nullptr};

  std::shared_ptr<app::ApplicationContext> actx;
  std::shared_ptr<AnnotatorController> controller;
};

#endif  // IMAGEANNOTATOR_GTKMM3_PROJECT_ANNOTATORCOTNROLLER_CT_BASE_CLASS_H