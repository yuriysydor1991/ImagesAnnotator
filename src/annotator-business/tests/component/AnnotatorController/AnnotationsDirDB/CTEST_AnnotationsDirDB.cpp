#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_set>

#include "ctests_decls.h"
#include "src/annotator-business/AnnotatorController.h"
#include "src/annotator-events/ImagesAnnotatorEventController.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/app/ApplicationContext.h"
#include "src/log/log.h"

namespace fs = std::filesystem;

using namespace testing;
using namespace iannotator;
using namespace events::events;

class CTEST_AnnotationsDirDB : public Test
{
 public:
  inline static const std::string tmp_data_dir =
      ctests_decls::tmp_data_path_root + "/CTEST_AnnotationsDirDB";

  ~CTEST_AnnotationsDirDB() = default;

  CTEST_AnnotationsDirDB()
      : actx{create_app_context(argc, argv)},
        controller{std::make_shared<AnnotatorController>()}
  {
    EXPECT_NE(controller, nullptr);
    EXPECT_NE(actx, nullptr);
  }

  inline static std::shared_ptr<app::ApplicationContext> create_app_context(
      int& argc, char**& argv)
  {
    auto c = std::make_shared<app::ApplicationContext>(argc, argv);

    c->eventer = std::make_shared<events::ImagesAnnotatorEventController>(c);

    return c;
  }

  int argc{0};
  char** argv{nullptr};

  std::shared_ptr<app::ApplicationContext> actx;
  std::shared_ptr<AnnotatorController> controller;
};

TEST_F(CTEST_AnnotationsDirDB, no_context_init_failure)
{
  EXPECT_FALSE(controller->init({}));
}

TEST_F(CTEST_AnnotationsDirDB, empty_context_init_success)
{
  EXPECT_TRUE(controller->init(actx));
}
