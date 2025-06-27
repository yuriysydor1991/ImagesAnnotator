#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "CTEST_AnnotatorController.h"
#include "ctests_decls.h"
#include "src/annotator-business/AnnotatorController.h"
#include "src/annotator-events/ImagesAnnotatorEventController.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/app/ApplicationContext.h"
#include "src/log/log.h"

using namespace testing;
using namespace iannotator;
using namespace events::events;

class CTEST_AnnotationsDirDB : public CTEST_AnnotatorController
{
 public:
  inline static const std::string tmp_data_dir =
      ctests_decls::tmp_data_path_root + "/CTEST_AnnotationsDirDB";

  ~CTEST_AnnotationsDirDB() = default;

  CTEST_AnnotationsDirDB()
  {
    EXPECT_NE(controller, nullptr);
    EXPECT_NE(actx, nullptr);
  }
};

TEST_F(CTEST_AnnotationsDirDB, no_context_init_failure)
{
  EXPECT_FALSE(controller->init({}));
}

TEST_F(CTEST_AnnotationsDirDB, empty_context_init_success)
{
  EXPECT_TRUE(controller->init(actx));
}
